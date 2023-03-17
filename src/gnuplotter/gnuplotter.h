/**
 * @file gnuplotter.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "gnuplot-iostream/gnuplot-iostream.h"

#include "map"
#include "string"
#include "deque"
#include "chrono"


class Gnuplotter {
private:
    Gnuplot _plot;
    std::chrono::time_point<std::chrono::system_clock> _timeStart;
    static constexpr size_t _dataSizeMax = 1000;
    std::map<std::string, std::deque<std::pair<unsigned long, double>>> _series;
public:
    Gnuplotter() {
        _timeStart = std::chrono::system_clock::now();


        //_plot << "set multiplot layout 2,1\n";
        //_plot << "set xrange [-2:2]\n";
        //_plot << "set yrange [-1:1]\n";
    }

    ~Gnuplotter() {
        _plot << "clear\n";
        _plot << "unset output ; exit gnuplot\n";
        system("pkill -x gnuplot_qt");
    }

    void push(std::string seriesName, double value) {
        if (_series[seriesName].size() >= _dataSizeMax)
        {
            _series[seriesName].pop_front();
        }
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _timeStart);
        _series[seriesName].emplace_back(time.count(), value);
    }

    void update() {
        const int N = 1000;
        static double theta = 0;

        static unsigned long i = 0;
        double alpha = (static_cast<double>(i++ % N)/N-0.5) * 10;
        double value = sin(alpha*8.0 + theta) * exp(-alpha*alpha/2.0);
        theta += 0.2;

        push("aaa", value);
        push("bbb", cos(value) + 1);

        //_plot << "plot '-' binary" << _plot.binFmt1d(_series["aaa"], "record") << "with lines notitle"
        //		<< ", '-' binary" << _plot.binFmt1d(_series["bbb"], "record") << "with lines notitle\n";
        //_plot.sendBinary1d(_series["aaa"]);
        //_plot.sendBinary1d(_series["bbb"]);

        _plot << "set term qt 0\n";
        _plot << "plot '-' binary" << _plot.binFmt1d(_series["aaa"], "record") << "with lines notitle\n";
        _plot.sendBinary1d(_series["aaa"]);

        _plot << "set term qt 1\n";
        _plot << "plot '-' binary" << _plot.binFmt1d(_series["bbb"], "record") << "with lines notitle\n";
        _plot.sendBinary1d(_series["bbb"]);

        _plot.flush();
    }
};



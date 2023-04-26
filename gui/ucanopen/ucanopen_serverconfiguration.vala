///
///
///


namespace Ucanopen {


[GtkTemplate (ui = "/gui/ucanopen/ucanopen_serverconfiguration.ui")]
public class ServerConfiguration : Adw.Bin
{
    [GtkChild]
    private unowned Adw.ToastOverlay toast_overlay;

    [GtkChild]
    private unowned Gtk.Button refreshabout_button;
    [GtkChild]
    private unowned Gtk.Label devicename_label;
    [GtkChild]
    private unowned Gtk.Label hardwareversion_label;
    [GtkChild]
    private unowned Gtk.Label firmwareversion_label;
    [GtkChild]
    private unowned Gtk.Label serialnumber_label;

    [GtkChild]
    private unowned Adw.ComboRow category_comborow;
    [GtkChild]
    private unowned Adw.ComboRow object_comborow;
    [GtkChild]
    private unowned Gtk.Button refreshvalue_button;
    [GtkChild]
    private unowned Adw.EntryRow value_entryrow;
    //[GtkChild]
    //private unowned Gtk.Button write_button;
    
    [GtkChild]
    private unowned Gtk.Button apply_button;
    [GtkChild]
    private unowned Gtk.Button restore_button;


    private const int _category_count_max = 32;
    private const int _category_str_size = 32;
    private string _categories[_category_count_max];
    private Gtk.StringList _categories_model;

    private const int _object_count_max = 32;
    private const int _object_str_size = 32;
    private string _objects[_object_count_max];
    private Gtk.StringList _objects_model;


    public ServerConfiguration() {}

    construct
    {
        refreshabout_button.clicked.connect(() => {
            string buf = string.nfill(32, '0');
            ucanopen_server_read_string(Backend.Ucanopen.server, "sys", "info", "device_name", 500, buf, 32);
            devicename_label.label = buf;
            ucanopen_server_read_string(Backend.Ucanopen.server, "sys", "info", "hardware_version", 500, buf, 32);
            hardwareversion_label.label = buf;
            ucanopen_server_read_string(Backend.Ucanopen.server, "sys", "info", "firmware_version", 500, buf, 32);
            firmwareversion_label.label = buf;
            ucanopen_server_read_numval(Backend.Ucanopen.server, "sys", "info", "serial_number", 500, buf, 32);
            serialnumber_label.label = buf;
        });


        for (int i = 0; i < _category_count_max; ++i)
        {
            _categories[i] = string.nfill(_category_str_size, '\0');
        }

        for (int i = 0; i < _object_count_max; ++i)
        {
            _objects[i] = string.nfill(_object_str_size, '\0');
        }

        _categories_model = new Gtk.StringList(null);
        category_comborow.model = _categories_model;
        _objects_model = new Gtk.StringList(null);
        object_comborow.model = _objects_model;

        int category_count = ucanopen_server_get_config_categories(Backend.Ucanopen.server,
                _categories, _category_count_max, _category_str_size);

        if (category_count != 0)
        {
            for (int i = 0; i < category_count; ++i)
            {
                _categories_model.append(_categories[i]);
            }

            int object_count = ucanopen_server_get_config_objects(Backend.Ucanopen.server,
                    _categories[category_comborow.selected], _objects, _object_count_max, _object_str_size);
            for (int i = 0; i < object_count; ++i)
            {
                _objects_model.append(_objects[i]);
            }

            category_comborow.notify["selected"].connect(on_category_selected);
            object_comborow.notify["selected"].connect(on_object_selected);
            refreshvalue_button.clicked.connect(on_value_refresh);
            value_entryrow.apply.connect(on_value_write);
        }

        apply_button.clicked.connect(() => {
            Adw.MessageDialog dialog = new Adw.MessageDialog((Gtk.Window)root,
                    "Warning!",
                    "All configuration parameters will be applied.");
            dialog.add_response("cancel", "Cancel");
            dialog.add_response("continue", "Continue");
            dialog.set_response_appearance("cancel", DESTRUCTIVE);
            dialog.set_response_appearance("continue", SUGGESTED);
            dialog.response["continue"].connect(() => {
                ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "save_all_parameters");
            });
            dialog.present();	
        });

        restore_button.clicked.connect(() => {
            Adw.MessageDialog dialog = new Adw.MessageDialog((Gtk.Window)root,
                    "Warning!",
                    "Default configuration parameters will be restored.");
            dialog.add_response("cancel", "Cancel");
            dialog.add_response("continue", "Continue");
            dialog.set_response_appearance("cancel", DESTRUCTIVE);
            dialog.set_response_appearance("continue", SUGGESTED);
            dialog.response["continue"].connect(() => {
                ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "restore_all_default_parameters");
            });
            dialog.present();		
        });
    }

    void on_object_selected()
    {
        string buf = string.nfill(32, '\0');
        ucanopen_server_read_numval(Backend.Ucanopen.server, Backend.Ucanopen.server_config_category,
                _categories[category_comborow.selected], _objects[object_comborow.selected],
                500, buf, 32);
        value_entryrow.text = buf;
    }

    void on_category_selected()
    {
        object_comborow.notify["selected"].disconnect(on_object_selected);
        _objects_model.splice(0, _objects_model.get_n_items(), null);
        int object_count = ucanopen_server_get_config_objects(Backend.Ucanopen.server,
                _categories[category_comborow.selected], _objects, _object_count_max, _object_str_size);
        for (int i = 0; i < object_count; ++i)
        {
            _objects_model.append(_objects[i]);
        }
        object_comborow.notify["selected"].connect(on_object_selected);
        on_object_selected();
    }

    void on_value_refresh()
    {
        string buf = string.nfill(32, '\0');
        ucanopen_server_read_numval(Backend.Ucanopen.server, Backend.Ucanopen.server_config_category,
                _categories[category_comborow.selected], _objects[object_comborow.selected],
                500, buf, 32);
        value_entryrow.text = buf;
    }

    void on_value_write()
    {
        float val;
        bool is_number = float.try_parse(value_entryrow.text, out val);
        if (is_number && value_entryrow.text.length != 0)
        {
            ucanopen_server_write(Backend.Ucanopen.server, Backend.Ucanopen.server_config_category,
                    _categories[category_comborow.selected], _objects[object_comborow.selected],
                    value_entryrow.text);
        }
        else
        {
            string message = string.join("", "Bad value: ", value_entryrow.text);
            Adw.Toast toast = new Adw.Toast(message);
            toast.timeout = 1;
            toast_overlay.add_toast(toast);
        }
    }
}


}



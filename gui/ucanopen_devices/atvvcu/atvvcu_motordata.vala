namespace AtvVcu {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcu/atvvcu_motordata.ui")]
public class MotorData : Adw.Bin {


    public MotorData() {}

    construct {
        Timeout.add(50, update);
    }

    public bool update() {
        update_lf_data();
        update_lb_data();
        update_rf_data();
        update_rb_data();
        return true;
    }

    private void update_lf_data() {

    }

    private void update_lb_data() {

    }
    
    private void update_rf_data() {

    }
    
    private void update_rb_data() {

    }
}    

}

namespace AtvVcu {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcu/atvvcu_motorcontrol.ui")]
public class MotorControl : Adw.Bin {
    [GtkChild]
    private unowned Adw.PreferencesGroup _group;

    public string title {
        get { return _group.title; }
        set { _group.title = value; }
    }
}

}

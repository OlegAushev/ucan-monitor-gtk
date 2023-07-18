namespace AtvVcm {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcm/atvvcm_motorcontrol.ui")]
public class MotorControl : Adw.Bin {
    [GtkChild]
    private unowned Adw.PreferencesGroup _group;

    public string title {
        get { return _group.title; }
        set { _group.title = value; }
    }
}

}

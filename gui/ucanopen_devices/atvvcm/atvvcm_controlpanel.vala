namespace AtvVcm {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcm/atvvcm_controlpanel.ui")]
public class ControlPanel : Adw.Bin {
    [GtkChild]
    private unowned Gtk.Button button_clear_errors;
    [GtkChild]
    private unowned Gtk.Button button_reset_device;

    public ControlPanel() {}

    construct {
        button_clear_errors.clicked.connect(() => {
            ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "clear_errors");
        });

        button_reset_device.clicked.connect(() => {
            Adw.MessageDialog dialog = new Adw.MessageDialog((Gtk.Window)root,
                    "Warning!",
                    "Device will be reset.");
            dialog.add_response("cancel", "Cancel");
            dialog.add_response("continue", "Continue");
            dialog.set_response_appearance("cancel", DESTRUCTIVE);
            dialog.set_response_appearance("continue", SUGGESTED);
            dialog.response["continue"].connect(() => {
                ucanopen_server_exec(Backend.Ucanopen.server, "sys", "ctl", "reset_device");
            });
            dialog.present();
        });
    }
}

}

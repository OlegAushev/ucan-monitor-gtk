namespace Ucanopen {

[GtkTemplate (ui = "/src/ui/assets/ucanopen/ucanopen_aboutserver.ui")]
public class AboutServer : Adw.Bin {
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

    public AboutServer() {}

    construct {
        refreshabout_button.clicked.connect(() => {
            string buf = string.nfill(32, '0');
            ucanopen_server_read_string(Backend.Ucanopen.server, "info", "sys", "device_name", 500, buf, 32);
            devicename_label.label = buf;
            ucanopen_server_read_string(Backend.Ucanopen.server, "info", "sys", "hardware_version", 500, buf, 32);
            hardwareversion_label.label = buf;
            ucanopen_server_read_string(Backend.Ucanopen.server, "info", "sys", "firmware_version", 500, buf, 32);
            firmwareversion_label.label = buf;
            ucanopen_server_read_numval(Backend.Ucanopen.server, "info", "sys", "serial_number", 500, buf, 32);
            serialnumber_label.label = buf;
        });
    }
}

}
    
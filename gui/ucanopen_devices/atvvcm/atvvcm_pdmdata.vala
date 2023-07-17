namespace AtvVcm {

[GtkTemplate (ui = "/gui/ucanopen_devices/atvvcm/atvvcm_pdmdata.ui")]
public class PdmData : Adw.PreferencesGroup {
    [GtkChild]
    private unowned BoolEntry battery_indicator;
    [GtkChild]
    private unowned BoolEntry motor1bypass_indicator;
    [GtkChild]
    private unowned BoolEntry motor2bypass_indicator;
    [GtkChild]
    private unowned BoolEntry motor3bypass_indicator;
    [GtkChild]
    private unowned BoolEntry motor4bypass_indicator;
    [GtkChild]
    private unowned BoolEntry chargemode_indicator;
    [GtkChild]
    private unowned BoolEntry allowcharge_indicator;
    [GtkChild]
    private unowned BoolEntry auxequipbypass_indicator;

    construct
    {
        Timeout.add(100, update);
    }

    public bool update()
    {
        battery_indicator.value = atvvcm_get_pdm_contactor_state(0);
        motor1bypass_indicator.value = atvvcm_get_pdm_contactor_state(1);
        motor2bypass_indicator.value = atvvcm_get_pdm_contactor_state(2);
        motor3bypass_indicator.value = atvvcm_get_pdm_contactor_state(3);
        motor4bypass_indicator.value = atvvcm_get_pdm_contactor_state(4);
        chargemode_indicator.value = atvvcm_get_pdm_contactor_state(5);
        allowcharge_indicator.value = atvvcm_get_pdm_contactor_state(6);
        auxequipbypass_indicator.value = atvvcm_get_pdm_contactor_state(7);
        return true;
    }
}

}


package burp.hexdump.yt;

import burp.api.montoya.BurpExtension;
import burp.api.montoya.MontoyaApi;
import burp.api.montoya.logging.Logging;

// Burp will auto-detect and load any class that extends BurpExtension.
public class Hexdump implements BurpExtension
{
    @Override
    public void initialize(MontoyaApi api)
    {
        // set extension name
        api.extension().setName("Hexdump Extension");
	api.userInterface().registerContextMenuItemsProvider(new MyContextMenuItemsProvider(api));
    }
}

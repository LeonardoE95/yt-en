package burp.hexdump.yt;

import burp.api.montoya.MontoyaApi;
import burp.api.montoya.logging.Logging;
import burp.api.montoya.core.ToolType;
import burp.api.montoya.http.message.HttpRequestResponse;
import burp.api.montoya.ui.contextmenu.ContextMenuEvent;
import burp.api.montoya.ui.contextmenu.ContextMenuItemsProvider;

import burp.api.montoya.http.message.requests.HttpRequest;
import burp.api.montoya.http.message.HttpHeader;
import burp.api.montoya.http.message.responses.HttpResponse;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.*;

import java.util.ArrayList;
import java.util.List;

public class MyContextMenuItemsProvider implements ContextMenuItemsProvider {
    private final MontoyaApi api;
    private Integer id;    

    public MyContextMenuItemsProvider(MontoyaApi api) {
        this.api = api;
	this.id = 1;
    }

    @Override
    public List<Component> provideMenuItems(ContextMenuEvent event) {
	
        if (event.isFromTool(ToolType.PROXY, ToolType.TARGET, ToolType.LOGGER, ToolType.REPEATER)) {
            List<Component> menuItemList = new ArrayList<>();

            HttpRequestResponse requestResponse;
	    if (event.messageEditorRequestResponse().isPresent()) {
		requestResponse = event.messageEditorRequestResponse().get().requestResponse();
	    } else {
		requestResponse = event.selectedRequestResponses().get(0);
	    }

            JMenuItem sendToRepeater = new JMenuItem("Send to Repeater");
	    sendToRepeater.addActionListener(l -> { customSendToRepeater(requestResponse); });

            JMenuItem copyToOrg = new JMenuItem("Copy to Org");
	    copyToOrg.addActionListener(l -> { copyToClipboardWithOrgFormat(requestResponse); });	    
	    
            menuItemList.add(sendToRepeater);
            menuItemList.add(copyToOrg);	    

            return menuItemList;
        }

        return null;
    }

    public void copyToClipboardWithOrgFormat(HttpRequestResponse requestResponse) {
	Clipboard c = Toolkit.getDefaultToolkit().getSystemClipboard();	    
	
	String requestData = requestResponse.request().toString().replace("\r", "");
	String responseData = requestResponse.response() != null ? 
	    requestResponse.response().toString().replace("\r", "") : "";

	StringBuilder data = new StringBuilder();
	data.append("Request:\n\n");
	data.append("#+begin_example\n");
	data.append(requestData);
	data.append("\n#+end_example\n\n");

	data.append("Response:\n\n");
	data.append("#+begin_example\n");
	data.append(responseData);
	data.append("\n#+end_example\n\n");		    

	StringSelection selection = new StringSelection(data.toString());
	c.setContents(selection, selection);
    }

    public void customSendToRepeater(HttpRequestResponse requestResponse) {	
	HttpRequest req = requestResponse.request();
	String method = req.method();
	String path = req.pathWithoutQuery();
	String id = this.id.toString();
	
	String tabName = id + " - " + method + " - " + path;
	this.api.repeater().sendToRepeater(req, tabName);
	
	this.id += 1;
    } 
}

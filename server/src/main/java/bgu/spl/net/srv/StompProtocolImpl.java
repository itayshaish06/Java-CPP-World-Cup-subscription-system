package bgu.spl.net.srv;

import bgu.spl.net.api.StompMessagingProtocol;

public class StompProtocolImpl<T> implements StompMessagingProtocol<T> {   

    private int connectionId;
    public ConnectionsImpl<T> connections;
    private ConnectionHandler<T> handler;
    private volatile boolean shouldDisconnect = false;

    // constructor
    public StompProtocolImpl() {
        connectionId = -1;
        connections = null;
    }

    public void start(int connectionId, Connections<T> connections, ConnectionHandler<T> handler) {
        this.connectionId = connectionId;
        this.connections = (ConnectionsImpl<T>) connections;
        this.connections.addConnection(connectionId, handler);
        this.handler = handler;
    }

    //get connectionId
    public int getConnectionId() {
        return connectionId;
    }

    public T process(T message) {
        String msg = (String) message;
        int endOfCommand = msg.indexOf('\n');
        String command = msg.substring(0, endOfCommand);
        msg = msg.substring(endOfCommand + 1);
        message = (T) msg;
        if (command.equals("CONNECT"))
            return (T)handleConnect(message);
        else if (command.equals("SUBSCRIBE"))
            return (T)handleSubscribe(message);
        else if (command.equals("UNSUBSCRIBE"))
            return (T)handleUnsubscribe(message);
        else if (command.equals("SEND"))
            return (T)handleSend(msg);
        else if (command.equals("DISCONNECT"))
            return (T)handleDisconnect(message);
        shouldDisconnect = true;
        return (T)ErrorFrame.malformedFrame("");
    }

    public boolean shouldTerminate() {
        return shouldDisconnect;
    }

    //handle connect command
    private String handleConnect(T message) {
        String connectMsg = (String) message;
        String accept_version = "";
        String host = "";
        String userName = "";
        String passcode = "";
        String receiptId = "";
        while (connectMsg.length() != 0) {
            int start = connectMsg.indexOf(':')+1;

            // if reach to end of message
            if (start == 0)
                break;

            String info = connectMsg.substring(0, start);
            int end = connectMsg.indexOf('\n');
            String value = connectMsg.substring(start, end);
            if (info.equals("accept-version:"))
                accept_version = value;
            else if (info.equals("host:"))
                host = value;
            else if (info.equals ("login:"))
                userName = value;
            else if (info.equals("passcode:"))
                passcode = value;
            else if (info.equals("receipt:"))
                passcode = value;
            connectMsg = connectMsg.substring(end + 1);
        }

        if ((!accept_version.equals("1.2")) || (!host.equals("stomp.cs.bgu.ac.il")) || userName.equals("")|| passcode.equals("")) {
            shouldDisconnect = true;
            return ErrorFrame.malformedFrame(receiptId);
        }
        //check if userName and passcode are valid
            //check if user is already logged in
            if (connections.isUserActive(userName)) {
                shouldDisconnect = true;
                return ErrorFrame.userAlreadyConnected(receiptId);
            }
            //check if user is registered
            if (connections.isUserExist(userName)) {
                if(!connections.isThePassCorrect(userName, passcode)){
                    shouldDisconnect = true;
                    return ErrorFrame.wrongPassword(receiptId);
                }
                else{
                    connections.addActiveUser(userName);
                    connections.updateUsernameOfConnectionID(connectionId, userName);
                }
            }
            else {
                //user isn't registered
                connections.addUserToUserToPass(userName, passcode);
                connections.addActiveUser(userName);
                connections.updateUsernameOfConnectionID(connectionId, userName);
            }
            if(!receiptId.equals("")){
                connections.send(connectionId, (T)"CONNECTED\nversion:1.2\n\n");
                return "RECEIPT\nreceipt-id:" + receiptId + "\n\n";
            }
        return "CONNECTED\nversion:1.2\n\n";
    }

    //handle subscribe command
    private String handleSubscribe(T message) {
        String subscribeMsg = (String) message;
        String destination = "";
        String receiptId = "";
        String id = "";
        while (subscribeMsg.length() != 0) {
            int start = subscribeMsg.indexOf(':')+1;

            // if reach to end of message
            if (start == 0)
                break;

            String info = subscribeMsg.substring(0, start);
            int end = subscribeMsg.indexOf('\n');
            String value = subscribeMsg.substring(start, end);
            if (info.equals("destination:"))
                destination = value;
            else if (info.equals("receipt:"))
                receiptId = value;
            else if (info.equals ("id:"))
                id = value;
            subscribeMsg = subscribeMsg.substring(end + 1);
        }
        if (destination.equals("") || id.equals("")) {
            shouldDisconnect = true;
            return ErrorFrame.malformedFrame(receiptId);
        }

        connections.subscribe(destination, id, connectionId);

        if(!receiptId.equals("")){
            return "RECEIPT\nreceipt-id:" + receiptId + "\n\n";
        }

        return null;
    }
    
    //handle unsubscribe command
    private String handleUnsubscribe(T message) {
        String unsubscribeMsg = (String) message;
        String receiptId = "";
        String id = "";
        while (unsubscribeMsg.length() != 0) {
            int start = unsubscribeMsg.indexOf(':')+1;

            // if reach to end of message
            if (start == 0)
                break;

            String info = unsubscribeMsg.substring(0, start);
            int end = unsubscribeMsg.indexOf('\n');
            String value = unsubscribeMsg.substring(start, end);
            if (info.equals("receipt:"))
                receiptId = value;
            else if (info.equals ("id:"))
                id = value;
            unsubscribeMsg = unsubscribeMsg.substring(end + 1);
        }
        if (id.equals("")) {
            shouldDisconnect = true;
            return ErrorFrame.malformedFrame(receiptId);
        }

        if(!connections.unsubscribe(id, connectionId)){
            shouldDisconnect = true;
            return ErrorFrame.userNotSubscribed(receiptId);
        }

        if(!receiptId.equals("")){
            return "RECEIPT\nreceipt-id:" + receiptId + "\n\n";
        }

        return null;
    }

    //handle send command
    private String handleSend(String message) {
        String sendMsg = message;
        String destination = "";
        String receiptId = "";
        String body = "";
        while (sendMsg.length() != 0) {
            int end = sendMsg.indexOf('\n');
            while (end == 0) { //if there is an empty line
                sendMsg = sendMsg.substring(1);
                end = sendMsg.indexOf('\n');
            }
            int start = sendMsg.indexOf(':')+1;
            // if reach to end of message
            if (start == 0)
                break;
            String info = sendMsg.substring(0, start);
            String value = sendMsg.substring(start, end);
            if (info.equals("destination:"))
                destination = value.substring(1);//without the first '/'
            else if (info.equals("receipt:"))
                receiptId = value;
            else if (info.equals ("user:")){
                body = "user:" + sendMsg.substring(start);
                break;
            }
            sendMsg = sendMsg.substring(end + 1);
        }
        
        if (destination.equals("") || body.equals("")) {
            shouldDisconnect = true;
            return ErrorFrame.malformedFrame(receiptId);
        }

        //check if topic is already exist
        if (!connections.isChannelExist(destination)) {
            shouldDisconnect = true;
            return ErrorFrame.invalidChannel(receiptId);
        }

        connections.send(destination, (T)connections.wrapMessage(body, destination, connectionId));

        if(!receiptId.equals("")){
            return "RECEIPT\nreceipt-id:" + receiptId + "\n\n";
        }
        
        return null;
    }

    //handle disconnect command
    private String handleDisconnect(T message) {
        shouldDisconnect = true;
        String disconnectMsg = (String) message;
        String receiptId = "";
        while (disconnectMsg.length() != 0) {
            int start = disconnectMsg.indexOf(':');

            // if reach to end of message
            if (start == -1)
                break;

            String info = disconnectMsg.substring(0, start);
            int end = disconnectMsg.indexOf('\n');
            String value = disconnectMsg.substring(start + 1, end);
            if (info.equals("receipt"))
                receiptId = value;
            disconnectMsg = disconnectMsg.substring(end + 1);
        }

        connections.disconnect(connectionId);
 
        if(!receiptId.equals("")){
            return "RECEIPT\nreceipt-id:" + receiptId + "\n\n";
        }
        return null;
    }

    
}

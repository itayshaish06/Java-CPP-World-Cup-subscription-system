package bgu.spl.net.srv;
import java.util.Collection;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedDeque;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<String,String> userToPass;
    private ConcurrentHashMap<Integer, connectionInfo<T>> connectionsIDtoInfo;
    private ConcurrentHashMap<String,String> activeUsers;
    private ConcurrentHashMap<String,ConcurrentLinkedDeque<Integer>> channelToConnectionID;
    private static int msgId = 0;

    //constructor
    public ConnectionsImpl() {
        this.userToPass = new ConcurrentHashMap<>();
        this.connectionsIDtoInfo = new ConcurrentHashMap<>();
        this.activeUsers = new ConcurrentHashMap<>();
        this.channelToConnectionID = new ConcurrentHashMap<>();
    }

    //add user to userToPass
    public void addUserToUserToPass(String username, String password) {
        userToPass.put(username, password);
    }

    //check if user exist in userToPass
    public boolean isUserExist(String username) {
        return userToPass.containsKey(username);
    }

    //check if password is correct
    public boolean isThePassCorrect(String username, String password) {
        return userToPass.get(username).equals(password);
    }

    //add connectionID to connectionsIDtoInfo
    public void addConnection(int connectionID, ConnectionHandler<T> myConnectionHandler) {
        connectionsIDtoInfo.put(connectionID, new connectionInfo<T>("", myConnectionHandler));
    }

    //update username of connectionID
    public void updateUsernameOfConnectionID(int connectionID, String username) {
        connectionsIDtoInfo.get(connectionID).setUsername(username);
    }

    //remove connectionID from connectionsIDtoInfo
    public void disconnect(int connectionID) {
        connectionInfo<T> myConnectionInfo = connectionsIDtoInfo.get(connectionID);
        Collection<String> Channels = myConnectionInfo.getConnectionChannels();
        for (String channel : Channels) {
            unsubscribeFromChannel(channel, connectionID);
        }
        connectionsIDtoInfo.remove(connectionID);
        removeActiveUser(myConnectionInfo.getUsername());
    }

    //get connectionInfo of connectionID
    public connectionInfo<T> getConnectionInfoOfConnectionID(int connectionID) {
        return connectionsIDtoInfo.get(connectionID);
    }

    //subscribe to channel
    public void subscribe(String channel, String subID, int connectionID) {
        subscribeToChannel(channel, connectionID);
        getConnectionInfoOfConnectionID(connectionID).addSubIDtoChannel(subID, channel);
    }

    //add connectionID from channelToConnectionID
    private void subscribeToChannel(String channel, int connectionID) {
        if (channelToConnectionID.containsKey(channel)) {
            channelToConnectionID.get(channel).add(connectionID);
        }
        else {
            ConcurrentLinkedDeque<Integer> newChannel = new ConcurrentLinkedDeque<>();
            newChannel.add(connectionID);
            channelToConnectionID.put(channel, newChannel);
        }
    }

    //unsubscribe from channel
    public boolean unsubscribe(String subID, int connectionID) {
        String channel = getConnectionInfoOfConnectionID(connectionID).getSubIDtoChannel(subID);
        if (channel == null)
            return false;
        unsubscribeFromChannel(channel, connectionID);
        getConnectionInfoOfConnectionID(connectionID).removeSubIDfromChannel(subID);
        return true;
    }

    //remove connectionID from channelToConnectionID
    private void unsubscribeFromChannel(String channel, int connectionID) {
        ConcurrentLinkedDeque<Integer> ChannelSubscribers = channelToConnectionID.get(channel);
        ChannelSubscribers.remove(connectionID);
        if(ChannelSubscribers.isEmpty()) {
            channelToConnectionID.remove(channel);
        }
    }

    //get connectionhandler of connectionID
    public ConnectionHandler<T> getConnectionHandlerOfConnectionID(int connectionID) {
        return connectionsIDtoInfo.get(connectionID).getMyConnectionHandler();
    }
    
    //add user to activeUsers
    public void addActiveUser(String username){
        activeUsers.put(username, username);
    }

    //remove user from activeUsers
    public void removeActiveUser(String username){
        activeUsers.remove(username);
    }

    //check if user is active
    public boolean isUserActive(String username){
        return activeUsers.containsKey(username);
    }

    public void send(String channel, T msg) {
        ConcurrentLinkedDeque<Integer> ChannelSubscribers = channelToConnectionID.get(channel);
        for (Integer connectionID : ChannelSubscribers) {
            ConnectionHandler<T> myConnectionHandler = getConnectionHandlerOfConnectionID(connectionID);
            try{
                myConnectionHandler.send(msg);
            }
            catch (Exception e){}
        }
    }

    public boolean send(int connectionId, T msg){
        ConnectionHandler<T> myConnectionHandler = getConnectionHandlerOfConnectionID(connectionId);
        try{
            myConnectionHandler.send(msg);
        }
        catch (Exception e){
            return false;
        }
        return true;
    }

    //is the channel exist
    public boolean isChannelExist(String channel) {
        return channelToConnectionID.containsKey(channel);
    }
    
    //wrap the message
    public String wrapMessage(String body, String channel, int connectionID) {
        String subID = getConnectionInfoOfConnectionID(connectionID).getSubIDtoChannel(channel);
        String message = "MESSAGE\nsubscription:" + subID + "\nmessage-id:"+ msgId +"\ndestination:" + channel + "\n\n" + body;
        msgId++;
        return message;
    }

}

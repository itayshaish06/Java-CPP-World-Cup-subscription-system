package bgu.spl.net.srv;

import java.util.Collection;
import java.util.concurrent.ConcurrentHashMap;

public class connectionInfo<T> {
    private String username;
    private ConnectionHandler<T> myConnectionHandler;
    private ConcurrentHashMap<String,String> subIDtoChannel;

    //constructor
    public connectionInfo(String username, ConnectionHandler<T> myConnectionHandler) {
        this.username = username;
        this.myConnectionHandler = myConnectionHandler;
        this.subIDtoChannel = new ConcurrentHashMap<>();
    }
    //getters
    public String getUsername() {
        return username;
    }

    //Set username
    public void setUsername(String username) {
        this.username = username;
    }

    public ConnectionHandler<T> getMyConnectionHandler() {
        return myConnectionHandler;
    }

    public String getSubIDtoChannel(String subId) {
        return subIDtoChannel.get(subId);
    }
    
    public Collection<String> getConnectionChannels() {
        return subIDtoChannel.values();
    }

    //add subID to channel
    public void addSubIDtoChannel(String subId, String channel) {
        this.subIDtoChannel.put(subId, channel);
    }
    //remove subID from channel
    public void removeSubIDfromChannel(String subId) {
        this.subIDtoChannel.remove(subId);
    }
}

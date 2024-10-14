package bgu.spl.net.api;
import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
public interface MessagingProtocol<T> {
 
    /**
     * process the given message 
     * @param msg the received message
     * @return the response to send or null if no response is expected by the client
     */
    T process(T msg);
 
    /**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();

 	/**
	 * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
	**/
    void start(int connectionId, Connections<T> connections, ConnectionHandler<T> handler);
}
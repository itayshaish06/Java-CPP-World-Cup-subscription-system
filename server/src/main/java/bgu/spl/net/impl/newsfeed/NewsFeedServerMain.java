package bgu.spl.net.impl.newsfeed;

import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.Server;

public class NewsFeedServerMain {

    public static void main(String[] args) {
        // NewsFeed feed = new NewsFeed(); //one shared object
        // ConnectionsImpl<String> connections = new ConnectionsImpl<String>();

        // // you can use any server... 
        // Server.threadPerClient(
        //         7777, //port
        //         () ->  new RemoteCommandInvocationProtocol<>(feed), //protocol factory
        //         ObjectEncoderDecoder::new, //message encoder decoder factory
        //         connections
        // ).serve();

        // Server.reactor(
        //         Runtime.getRuntime().availableProcessors(),
        //         7777, //port
        //         () ->  new RemoteCommandInvocationProtocol<>(feed), //protocol factory
        //         ObjectEncoderDecoder::new //message encoder decoder factory
        // ).serve();
    }
}

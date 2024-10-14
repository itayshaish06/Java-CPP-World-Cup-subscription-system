package bgu.spl.net.impl.stomp;

import java.util.Scanner;

import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.Server;
import bgu.spl.net.srv.StompMessageEncoderDecoder;
import bgu.spl.net.srv.StompProtocolImpl;

public class StompServer {

    public static void main(String[] args) {

        ConnectionsImpl<String> connections = new ConnectionsImpl<String>();

        if(args[1].equals("tpc")){
        Server.threadPerClient(
                Integer.parseInt(args[0]), //port
                () -> new StompProtocolImpl<String>(), //protocol factory
                StompMessageEncoderDecoder::new, //message encoder decoder factory
                connections
        ).serve();
        }
        else if(args[1].equals("reactor")){
            Server.reactor(
                    3,
                    Integer.parseInt(args[0]), //port
                    () -> new StompProtocolImpl<String>(), //protocol factory
                    StompMessageEncoderDecoder::new, //message encoder decoder factory
                    connections
            ).serve();
        }
        else{
            System.out.println("wrong input");
        }
    }
}

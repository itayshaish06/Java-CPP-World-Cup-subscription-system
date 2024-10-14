package bgu.spl.net.tests;

public class test {
    //test stompProtocolImpl handleConnect
    public static void main(String[] args) {
        String connectMsg = "accept_version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:guest\npasscode:guest\n";
        String accept_version = "";
        String host = "";
        String userName = "";
        String passcode = "";
        String response = "";
        boolean validConnect = true;
        while (connectMsg.length() != 0) {
            int start = connectMsg.indexOf(':');

            // if reach to end of message
            if (start == -1)
                break;

            String info = connectMsg.substring(0, start);
            int end = connectMsg.indexOf('\n');
            String value = connectMsg.substring(start, end);
            if (info.equals("accept_version"))
                accept_version = value;
            else if (info.equals("host"))
                host = value;
            else if (info.equals ("login"))
                userName = value;
            else if (info.equals("passcode"))
                passcode = value;
            connectMsg = connectMsg.substring(end + 1);
        }

        if ((!accept_version.equals(":1.2")) || (!host.equals(":stomp.cs.bgu.ac.il")) || userName.equals("")|| passcode.equals("")) {
            validConnect = false;
            System.out.println("ErrorFrame.malformedFrame()");
        }
        System.out.println(validConnect);
        System.out.println(response);
    }

}

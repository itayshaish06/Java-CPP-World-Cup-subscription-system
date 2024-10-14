package bgu.spl.net.srv;

public class ErrorFrame {

    //error in the connect frame
    public static String wrongPassword(String receiptId) {
        if(receiptId.equals(""))
            return "ERROR\n" +"message: Wrong password\n";
        return "ERROR\n" + "receipt-id" + receiptId + "\n" + "message: Wrong password\n";
    }

    //error in the connect frame
    public static String userAlreadyConnected(String receiptId) {
        if(receiptId.equals(""))
            return "ERROR\n" + "message: User already connected\n";
        return "ERROR\n" + "receipt-id" + receiptId + "\n" + "message: User already connected\n";
    }

    // error in frame (malformed frame)
    public static String malformedFrame(String receiptId) {
        if(receiptId.equals(""))
            return "ERROR\n" + "message: Malformed frame\n";
        return "ERROR\n" + "receipt-id" + receiptId + "\n" + "message: Malformed frame\n";
    }
    
    //error in the send frame (user not subscribed to the channel)
    public static String userNotSubscribed(String receiptId) {
        if(receiptId.equals(""))
            return "ERROR\n" + "message: User not subscribed\n";
        return "ERROR\n" + "receipt-id" + receiptId + "\n" + "message: User not subscribed\n";
    }

    // error in the send frame (missing destination)
    public static String missingDestination(String receiptId) {
        if(receiptId.equals(""))
            return "ERROR\n" + "message: Missing destination\n";
        return "ERROR\n" + "receipt-id" + receiptId + "\n" + "message: Missing destination\n";
    }

    // error in the send frame (user tries to send to invalid channel)
    public static String invalidChannel(String receiptId) {
        if(receiptId.equals(""))
            return "ERROR\n" + "message: invalid Channel\n";
        return "ERROR\n" + "receipt-id" + receiptId + "\n" + "message: invalid Channel\n";
    }

    // error in subscribe & unsubscribed frame (missing subscription ID)
    public static String missingSubscriptionId(String receiptId) {
        if(receiptId.equals(""))
            return "ERROR\n" + "message: Missing subscription ID\n";
        return "ERROR\n" + "receipt-id" + receiptId + "\n" + "message: Missing subscription ID\n";
    }

}

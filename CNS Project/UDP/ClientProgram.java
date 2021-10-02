import java.io.IOException;
import java.util.Scanner;

public class ClientProgram implements OnSocketListener {
    private Client client;
    private Boolean start = false;
    private Boolean talk = false;

	@Override
	public void onConnected(Channel channel) {
		System.out.println("Server Connected.");
	}

	@Override
	public void onDisconnected(Channel channel) {
		System.out.println("Disconnected.");
	}

	@Override
	public void onReceived(Channel channel, String msg) {
		// System.out.println(msg);
		// get the command
		String[] command = msg.split(" ");
		
		if (command[2].equals("/NoUserFound")) {
			System.out.println("No User Found");
        } else if (command[2].equals("/SelfPort")) {
            String talkingname = command[3];
			int talkingport = Integer.parseInt(command[4]);
            client.setTalking(talkingname, talkingport);
            System.out.println("Start talking with " + talkingname + "(yourself)");
            talk = true;
        } else if (command[2].equals("/TargetPort")) {
            String talkingname = command[3];
			int talkingport = Integer.parseInt(command[4]);
			client.setTalking(talkingname, talkingport);
            System.out.println("Start talking with " + talkingname);
            talk = true;
		} else if (command[2].equals("/quit")) {
			System.out.println("Quit talking with " + client.getTalkingName());
            client.setTalking(null, 0);
            talk = false;
        } else if (command[2].equals("/NameExisted")) {
            System.out.println("Name existed, please enter a new name");
        } else if (command[2].equals("/SuccessRegister")) {
            start = true;
            System.out.println("User Register successfully");
        } else if (command[2].equals("/TargetUserOccupied")) {
            System.out.println("Target user is occupied, please try later");
        } else if (command[2].equals("/WrongInstruction")) {
            System.out.println("Wrong instruction");
        } else {
            System.out.println("Unknown command : " + msg);
        }
    }

    public void start() throws IOException {
        Scanner scanner = new Scanner(System.in);
        // Input Name
        System.out.print("Name : ");
        String name = scanner.nextLine();
        // Constant
        String ip = "127.0.0.1";
        int port = 20000;
        // Input UDP Port
        System.out.print("Local UDP Port : ");
        int UdpPort = Integer.parseInt(scanner.nextLine());
        // start a client
        client = new Client(name, this, ip, port);
        client.connect(UdpPort);
        // let the program wait a litte bit
        try { Thread.sleep(500); } catch (Exception e) { System.exit(0); }
        while (true) {
            // check name
            client.send("/check " + name);
            try { Thread.sleep(500); } catch (Exception e) { System.exit(0); }
            if (start == true)
                break;
            // Case: name existed
            System.out.print("Name : ");
            name = scanner.nextLine();
            client.setName(name);
        }
		// connect to the server
		client.send("/add " + name + " " + UdpPort);
		
        while (true) {
			// get the input string
            String msg = null;
            // Case: conversation existed
            msg = scanner.nextLine();
            if (talk == true) {
                if (msg.isEmpty())
                    continue;
                if (msg.charAt(0) != '/') {
                    if (client.getTalkingPort() != 0) {
                        client.say(msg);
                    }
                } else if (msg.split(" ")[0].equals("/quit")) {
                    client.send("/quit " + client.getTalkingName());
                } else {
                    System.out.println("Wrong instruction (only accept \"/quit\")");
                }
            } else {
                if (msg.isEmpty())
                    break;
                // Otherwise
                if (msg.charAt(0) == '/') {
                    client.send(msg);
                } else {
                    System.out.println("Wrong instruction");
                }
            }
		}
        client.send("/terminate");

		scanner.close();
		client.stop();
	}

	public static void main(String[] args) throws IOException {
		ClientProgram program = new ClientProgram();
		program.start();
	}
}

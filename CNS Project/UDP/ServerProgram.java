import java.io.IOException;
import java.net.Socket;
import java.util.Scanner;

public class ServerProgram implements OnSocketListener {
    private Server server;

	@Override
	public void onConnected(final Channel channel) {
        final Socket socket = channel.getSocket();
        final String hostName = socket.getInetAddress().getHostName();
        final int port = socket.getPort();

        final String msg = "Client connected from " + hostName + ":" + port;

        System.out.println(msg);
    }

    @Override
    public void onDisconnected(final Channel channel) {
        if (channel.getName() != null) {
            server.removeUser(channel.getName());
        }
        server.remove(channel);

        final Socket socket = channel.getSocket();
        final String hostName = socket.getInetAddress().getHostName();
        final int port = socket.getPort();

        final String msg = "Client disconnected from " + hostName + ":" + port;

        System.out.println(msg);
    }

    @Override
    public void onReceived(final Channel channel, final String msg) {
        System.out.println(msg);
        final String[] command = msg.split(" ");
        final String sourceName = command[0];
        final Channel sourceChannel = server.getChannel(sourceName);

        if (command[2].equals("/add")) {
            // add user and add channel
            final String name = command[3];
            final String port = command[4];
            server.addUser(name, port);
            server.addChannel(name, channel);
            // channel.setName(name);
        } else if (command[2].equals("/check")) {
            // check name
            final String targetName = command[3];
            final String targetPort = server.getUser(targetName);
            if (targetPort != null) {
                channel.send("server >> /NameExisted");
            } else {
                channel.send("server >> /SuccessRegister");
            }
        } else if (command[2].equals("/connect")) {
            // the client wants to connect another client
            final String targetName = command[3];
            final Channel targetChannel = server.getChannel(targetName);
            final String sourcePort = server.getUser(sourceName);
            final String targetPort = server.getUser(targetName);
            if (targetPort == null) {
                sourceChannel.send("server >> /NoUserFound");
            } else if (server.getUserState(targetName) == true) {
                sourceChannel.send("server >> /TargetUserOccupied");
            } else if (targetName.equals(sourceName)) {
                targetChannel.send("server >> /SelfPort " + targetName + " " + targetPort);
                server.setUserState(targetName, true);
            } else {
                targetChannel.send("server >> /TargetPort " + sourceName + " " + sourcePort);
                sourceChannel.send("server >> /TargetPort " + targetName + " " + targetPort);
                server.setUserState(sourceName, true);
                server.setUserState(targetName, true);
            }
        } else if (command[2].equals("/quit")) {
            final String targetName = command[3];
            final Channel targetChannel = server.getChannel(targetName);
            sourceChannel.send("server >> /quit");
            server.setUserState(sourceName, false);
            if (!targetName.equals(sourceName)) {
                targetChannel.send("server >> /quit");
                server.setUserState(targetName, false);
            }
        } else if (command[2].equals("/terminate")) {
            server.removeChannel(sourceName);
            server.removeUser(sourceName);
        } else {
            sourceChannel.send("server >> /WrongInstruction");
        }
    }

    public void start() throws IOException {
        final Scanner scanner = new Scanner(System.in);
        final int port = 20000;

        server = new Server(this);
        server.bind(port);
        server.start();

        while (true) {
            String msg = scanner.nextLine();
            if (msg.isEmpty())
                break;
            // Otherwise
            msg = "Server >> " + msg;
            System.out.println(msg);
            server.broadcast(msg);
        }

        scanner.close();
        server.stop();
    }

    public static void main(final String[] args) throws IOException {
        ServerProgram program = new ServerProgram();
		program.start();
	}
}

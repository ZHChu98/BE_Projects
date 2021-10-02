import java.io.IOException;
import java.net.Socket;
import java.net.*;
public class Client
{
	private String name;
	private Channel channel;
	private UdpChannel udpChannel;
	private String talkingName;
	private int talkingPort;
	private DatagramSocket udpSocket;
	private int UdpPort;
	private OnSocketListener onSocketListener;
	
	public Client(String name, OnSocketListener onSocketListener, String ip, int port) throws IOException
	{
		this.name = name;
		this.onSocketListener = onSocketListener;
        this.talkingPort = 0;
        Socket socket = new Socket(ip, port);
		channel = new Channel(socket, onSocketListener);
        channel.start();
	}
	
	public void connect(int UdpPort) throws IOException
	{
        this.UdpPort = UdpPort;
        udpSocket = new DatagramSocket(UdpPort);
		udpChannel = new UdpChannel(udpSocket);
		udpChannel.start();
	}
    
    public void setName(String newName)
	{
        this.name = newName;
	}

	public void setTalking(String name, int port)
	{
		this.talkingName = name;
		this.talkingPort = port;
	}
	
	public int getTalkingPort()
	{
		return this.talkingPort;
	}
	
	public String getTalkingName()
	{
		return this.talkingName;
	}

	public void stop() throws IOException
	{
		channel.stop();
		udpChannel.stop();
	}
	
	public void send(String msg)
	{
		channel.send(name + " >> " + msg);
	}

	public void say(String msg) throws IOException
	{
		DatagramSocket s = new DatagramSocket();
		byte[] sendData = new byte[1024];
      	sendData = (name + " >> " + msg).getBytes();
      	DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, InetAddress.getByName("127.0.0.1"), talkingPort);
        s.send(sendPacket);
        s.close();
	}
}

package client;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import ui.GraphicsManager;

public class MessageClient {
    // User name, one client per user
    private String name;

    // Network socket, establish connection
    private Socket socket;

    // Reader, thrown in a buffer
    private BufferedReader reader;

    // Writer
    private PrintWriter writer;

    public MessageClient(String name, String host, int port) throws IOException {
        this.name = name;
        this.socket = new Socket(host, port);

        this.reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.writer = new PrintWriter(socket.getOutputStream(), true);

        System.out.println("Connected to server at " + host + " : " + port);
    }

    public void startListening(GraphicsManager gm) {
        Thread t = new Thread(() -> {
            try {
                String incoming;
                while ((incoming = reader.readLine()) != null) {
                    gm.ShowIncomingMessage(incoming, gm.GetChatDisplay());
                }
            } catch (IOException e) {
                gm.ShowIncomingMessage("[System] Disconnected from server", gm.GetChatDisplay());
            }
        });
        t.setDaemon(true);
        t.start();
    }

    // Throw a new message through the writer.
    public void WriteMessage(String msg) {
        writer.println(name + ": " + msg);
    }

    // Destablish connection.
    public void CloseSocket() throws IOException {
        socket.close();
    }
}

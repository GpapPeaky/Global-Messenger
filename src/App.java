import client.MessageClient;
import ui.GraphicsManager;

public class App {
    public static void main(String[] args) {
        try {
            GraphicsManager gm = new GraphicsManager();

            // Client
            MessageClient mc = new MessageClient("Peaky", "120.0.0.7", 8080);
            
            gm.Init(mc);
            
            gm.Show();
        } catch (Exception e) {
            System.out.println("Error connecting to server: " + e.getMessage());
        }
    }
}

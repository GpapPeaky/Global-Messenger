package ui;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import java.awt.Font;
import java.awt.Color;

import client.MessageClient;

public class GraphicsManager {
    // Initial frame width
    int FRAME_WIDTH = 600;

    // Initial frame height
    int FRAME_HEIGHT = 700;

    // Frame
    public JFrame frame;

    private StylizedButton sendMsg;
    private StylizedTextArea msgField;
    private JTextArea chatDisplay;
    private ImageManager im;

    // Frame creation
    public void Init(MessageClient client) {
        im = new ImageManager();

        JFrame frame = new JFrame("MsgApp");
        frame.setSize(FRAME_WIDTH, FRAME_HEIGHT);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(null);
        frame.setResizable(false);

        this.frame = frame;

        // Chat display
        chatDisplay = new JTextArea();
        chatDisplay.setEditable(false);
        chatDisplay.setFont(new Font(StylizedTextArea.FONT_NAME, Font.PLAIN, 16));
        chatDisplay.setBackground(Color.BLACK);
        chatDisplay.setForeground(Color.WHITE);
        chatDisplay.setBounds(10, 10, 560, 500);
        frame.add(chatDisplay);

        // Send message button
        ImageIcon sendMsgIcon = im.loadImage("src/assets/buttons/sendMsg.png", 50, 50);
        sendMsg = new StylizedButton(
                sendMsgIcon,
                frame.getWidth() - 70,
                frame.getHeight() - 95
            );

        // Text area
        ImageIcon textAreaIcon = im.loadImage("src/assets/buttons/textPanel.png", 520, 50);
        StylizedTextArea msgField = new StylizedTextArea(
                textAreaIcon,
                5,
                frame.getHeight() - 95,
                sendMsg
            );

        sendMsg.setButtonAction(() -> {
            String text = msgField.getText().trim();
            if (!text.isEmpty()) {
                client.WriteMessage(text); // send to C backend

                displayOwnMessage(text);
                msgField.setText("");
            }
        });
        
        sendMsg.AddTo(frame);
        msgField.AddTo(frame);

    }

    public void Show() {
        this.frame.setVisible(true);
    }

    // Read from client
    public void ShowIncomingMessage(String msg, JTextArea chatDisplay) {
        SwingUtilities.invokeLater(() -> chatDisplay.append(msg + "\n"));
    }

    // For my messages
    public void displayOwnMessage(String msg) {
        SwingUtilities.invokeLater(() -> chatDisplay.append("[You] " + msg + "\n"));
    }

    public JTextArea GetChatDisplay() {
        return chatDisplay;
    }
}

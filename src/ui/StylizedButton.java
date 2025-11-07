package ui;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.*;

public class StylizedButton extends JButton {
    // Constructor
    public StylizedButton(ImageIcon img, int x, int y) {
        super(img);

        setContentAreaFilled(false);
        setBorder(null);
        setBounds(x, y, img.getIconWidth(), img.getIconHeight());

        // Mouse press
        addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                Kickback();
            }
        });
        
        // Key binding for Enter
        getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke("ENTER"), "enterPressed");
        getActionMap().put("enterPressed", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Kickback(); // Animate
                if (actionListener != null) ((Runnable) actionListener).run(); // Peaky: Stinky poopy code ngl.
            }
        });
        
        setVisible(true);
    }

    // Button action
    public void setButtonAction(Runnable action) {
        // Mouse click triggers action
        addActionListener(e -> action.run());

        // Key binding for Enter
        getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke("ENTER"), "enterPressed");
        getActionMap().put("enterPressed", new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                action.run();
            }
        });
    }

    // Add component to the frame
    public void AddTo(JFrame frame) {
        frame.add(this); // Add yourself to the frame
        frame.revalidate();
        frame.repaint();
    }

    public void Kickback() {
        final int originalX = getX();
        final int originalY = getY();
    
        // Move down/right
        setLocation(originalX + 2, originalY + 2);
    
        // Reset after 50ms
        new javax.swing.Timer(50, e -> {
            setLocation(originalX, originalY);
            ((javax.swing.Timer)e.getSource()).stop();
        }).start();
    }
}

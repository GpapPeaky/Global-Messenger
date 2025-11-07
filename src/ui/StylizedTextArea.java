package ui;
import javax.swing.JTextArea;

import javax.swing.ImageIcon;
import javax.swing.JFrame;

import java.awt.Insets;
import java.awt.Color;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

public class StylizedTextArea extends JTextArea {
    // Message character limit
    private final int CHAR_LIMIT = 64;

    // Max font size 
    private final int MAX_FONT_SIZE = 30;

    // Min font size
    private final int MIN_FONT_SIZE = 12;

    // Font used
    static final String FONT_NAME = "Consolas";

    // Internal text padding (space from edges of image)
    private final int TEXT_PADDING_LEFT = 50;
    private final int TEXT_PADDING_RIGHT = 50;
    private final int TEXT_PADDING_TOP = 10;
    private final int TEXT_PADDING_BOTTOM = 10;

    // Background image
    private ImageIcon backgroundImg;

    // Constructor
    public StylizedTextArea(ImageIcon img, int x, int y, StylizedButton buttonAnimationInvoke) {
        super();

        this.backgroundImg = img;
        setForeground(Color.WHITE);
        setLineWrap(true);
        setWrapStyleWord(true);
        setFont(new Font(FONT_NAME, Font.PLAIN, MAX_FONT_SIZE));

        setOpaque(false);
        setBorder(null);

        // Component bounds
        setBounds(x, y, img.getIconWidth(), img.getIconHeight());

        // Actual text area bounds
        setMargin(new Insets(TEXT_PADDING_TOP, TEXT_PADDING_LEFT, TEXT_PADDING_BOTTOM, TEXT_PADDING_RIGHT));

        this.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    e.consume();
                    System.out.println(getText());
                    
                    setText("");
                    
                    UpdateFontSize();

                    buttonAnimationInvoke.Kickback(); // Animate
                }
            }

            // Message character limit
            @Override
            public void keyTyped(KeyEvent e) {
                if (getText().length() >= CHAR_LIMIT && e.getKeyChar() != KeyEvent.VK_BACK_SPACE) {
                    e.consume();
                }

                // Slight delay, so we catch up to the newest message length
                javax.swing.SwingUtilities.invokeLater(() -> UpdateFontSize());
            }
        });
        
        setVisible(true);
    }

    // Draw the text proportional to its size
    void UpdateFontSize() {
        if (getText().isEmpty()) return;
    
        int availableWidth = getWidth() - TEXT_PADDING_LEFT - TEXT_PADDING_RIGHT;
        int availableHeight = getHeight() - TEXT_PADDING_TOP - TEXT_PADDING_BOTTOM;
    
        int fontSize = MAX_FONT_SIZE;
        Font testFont;
        FontMetrics fm;
        int textHeight;
    
        do {
            testFont = new Font(FONT_NAME, Font.PLAIN, fontSize);
            fm = getFontMetrics(testFont);
            textHeight = fm.getHeight() * Math.max(1, getLineCount());
            fontSize--;
        } while ((textHeight > availableHeight || fm.stringWidth(getText()) > availableWidth) && fontSize > MIN_FONT_SIZE);
    
        setFont(testFont);
        revalidate();
        repaint();
    }

    @Override
    protected void paintComponent(Graphics g) {
        if (backgroundImg != null) {
            g.drawImage(backgroundImg.getImage(), 0, 0, getWidth(), getHeight(), this);
        }

        Graphics g2 = g.create();

        FontMetrics fm = g2.getFontMetrics(getFont());
        int textHeight = fm.getHeight() * Math.max(1, getLineCount());
        int availableHeight = getHeight() - TEXT_PADDING_TOP - TEXT_PADDING_BOTTOM;
        int verticalOffset = Math.max(0, (availableHeight - textHeight) / 2);

        g2.translate(TEXT_PADDING_LEFT, TEXT_PADDING_TOP + verticalOffset);

        super.paintComponent(g2);
        g2.dispose();
    }

    // Add component to the frame
    public void AddTo(JFrame frame) {
        frame.add(this); // Add yourself to the frame
        frame.revalidate();
        frame.repaint();
    }
}

package ui;
import java.awt.Image;

import javax.swing.ImageIcon;

public class ImageManager {
    public ImageIcon loadImage(String p, int w, int h) {
        ImageIcon originalIcon = new ImageIcon(p);

        Image resizedImg = originalIcon.getImage().getScaledInstance(w, h, Image.SCALE_SMOOTH);

        return new ImageIcon(resizedImg);
    }
}

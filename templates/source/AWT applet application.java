/*==========================================================================*/
/*
@@INCLUDE:$\templates\source\header-base@@
*/
/*--------------------------------------------------------------------------*/
/*
@@INCLUDE:$\templates\source\header-gnu@@
*/
/*--------------------------------------------------------------------------*/
/*
@@INCLUDE:$\templates\source\header-cvs@@
*/
/*==========================================================================*/
import java.awt.*;
import java.applet.*;
/*==========================================================================*/
/*
<applet code="@@FILE_NAME@@" width=300 height=300></applet>
*/
/*==========================================================================*/
@@INCLUDE:$\templates\source\header-java@@
public class @@FILE_NAME@@ extends Applet
{
    /**
     * Called by the browser or applet viewer to inform this applet that it has
     * been loaded into the system. It is always called before the first time that
     * the start method is called.
     *
     * @return Nothing.
     */
    public void init()
    {
        @@HERE@@this.repaint();
    }

    /**
     * Called by the browser or applet viewer to inform this applet that it is being
     * reclaimed and that it should destroy any resources that it has allocated. The
     * stop method will always be called before destroy.
     *
     * @return Nothing.
     */
    public void destroy()
    {

    }

    /**
     * Called by the browser or applet viewer to inform this applet that it should
     * start its execution. It is called after the init method and each time the
     * applet is revisited in a Web page.
     *
     * @return Nothing.
     */
    public void start()
    {

    }

    /**
     * Called by the browser or applet viewer to inform this applet that it should
     * stop its execution. It is called when the Web page that contains this applet
     * has been replaced by another page, and also just before the applet is to be
     * destroyed.
     *
     * @return Nothing.
     */
    public void stop()
    {

    }

    /**
     * Paints this component.
     *
     * @param g The graphics context to use for painting.
     * @return Nothing.
     */
    public void paint(Graphics g)
    {
        g.drawString("Hello World!",20,20);
    }

    /**
     * Updates this component.
     *
     * @param g The graphics context to use for updating.
     * @return Nothing.
     */
    public void update(Graphics g)
    {
        this.paint(g);
    }
}
/*==========================================================================*/

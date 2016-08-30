#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QtWidgets/QLabel>

/*!
 * A label extended with the possibility to retrieve click events.
 */
class ClickLabel : public QLabel {
    Q_OBJECT

public:
    /*!
     * Creates a new instance with the given \a parent.
     *
     * \param parent The parent of the label.
     */
    explicit ClickLabel(QWidget* parent=NULL);

    /*!
     * Frees all required resources upon deconstruction.
     */
    virtual ~ClickLabel();

signals:
    /*!
     * Signal emitted as soon as a click has been registered on the label.
     */
    void clicked();

protected:
    /*!
     * Retrieves the event of releasing a mouse button on the de
     *
     * \param event
     */
    void mouseReleaseEvent(QMouseEvent* event);
};

#endif // CLICKLABEL_H

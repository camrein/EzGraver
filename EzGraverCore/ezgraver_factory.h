#ifndef EZGRAVER_FACTORY_H
#define EZGRAVER_FACTORY_H

#include <QString>
#include <QStringList>

#include <memory>
#include <vector>

#include "ezgraver.h"

namespace Ez {

/*!
 * Creates an instance and connects to the given \a portName.
 *
 * \param portName The port the connection should be established to.
 * \param protocol The protocol version to use.
 * \return An instance of the EzGraver as a shared pointer.
 */
EZGRAVERCORESHARED_EXPORT std::shared_ptr<EzGraver> create(QString const& portName, int protocol = 1);

/*!
 * Gets the available protocols.
 *
 * \return The available protocols.
 */
EZGRAVERCORESHARED_EXPORT std::vector<int> protocols();

/*!
 * Gets a list of all available ports.
 *
 * \return A list with all ports.
 */
EZGRAVERCORESHARED_EXPORT QStringList availablePorts();

}


#endif // EZGRAVER_FACTORY_H

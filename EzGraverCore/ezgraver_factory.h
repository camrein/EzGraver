#ifndef EZGRAVER_FACTORY_H
#define EZGRAVER_FACTORY_H

#include <QString>
#include <memory>

#include "protocol.h"
#include "ezgraver.h"

namespace Ez {

/*!
 * Creates an instance and connects to the given \a portName.
 *
 * \param portName The port the connection should be established to.
 * \return An instance of the EzGraver as a shared pointer.
 */
EZGRAVERCORESHARED_EXPORT std::shared_ptr<EzGraver> create(QString const& portName, Protocol protocol = Protocol::v1);

}


#endif // EZGRAVER_FACTORY_H

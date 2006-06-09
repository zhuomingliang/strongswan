/**
 * @file x509.h
 * 
 * @brief Interface of x509_t.
 * 
 */

/*
 * Copyright (C) 2006 Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#ifndef X509_H_
#define X509_H_

#include <types.h>
#include <definitions.h>
#include <crypto/rsa/rsa_public_key.h>
#include <utils/identification.h>
#include <utils/iterator.h>
#include <utils/logger.h>


typedef struct x509_t x509_t;

/**
 * @brief X509 certificate.
 * 
 * @b Constructors:
 *  - x509_create_from_chunk()
 *  - x509_create_from_file()
 * 
 * @todo more code cleanup needed!
 * @todo fix unimplemented functions...
 * @todo handle memory management
 *
 * @ingroup transforms
 */
struct x509_t {

	/**
	 * @brief Get the RSA public key from the certificate.
	 * 
	 * @param this				calling object
	 * @return					public_key
	 */
	rsa_public_key_t *(*get_public_key) (const x509_t *this);
		
	/**
	 * @brief Get the certificate issuers ID.
	 * 
	 * The resulting ID is always a identification_t
	 * of type ID_DER_ASN1_DN.
	 * 
	 * @param this				calling object
	 * @return					issuers ID
	 */
	identification_t *(*get_issuer) (const x509_t *this);
		
	/**
	 * @brief Get the subjects ID.
	 * 
	 * The resulting ID is always a identification_t
	 * of type ID_DER_ASN1_DN. 
	 * 
	 * @param this				calling object
	 * @return					subjects ID
	 */
	identification_t *(*get_subject) (const x509_t *this);
	
	/**
	 * @brief Check if a certificate is valid.
	 * 
	 * This function uses the issuers public key to verify 
	 * the validity of a certificate.
	 * 
	 * @todo implement!
	 */
	bool (*verify) (const x509_t *this, rsa_public_key_t *signer);
	
	/**
	 * @brief Get the key identifier of the public key.
	 * 
	 * @todo implement!
	 */
	chunk_t (*get_subject_key_identifier) (const x509_t *this);
	
	/**
	 * @brief Compare two certificates.
	 * 
	 * Comparison is done via the certificates signature.
	 * 
	 * @param this			first cert for compare
	 * @param other			second cert for compare
	 * @return			TRUE if signature is equal
	 */
	bool (*equals) (const x509_t *this, const x509_t *that);
	
	/**
	 * @brief Checks if the certificate contains a subjectAltName equal to id.
	 * 
	 * @param this			certificate being examined
	 * @param id			id which is being compared to the subjectAltNames
	 * @return			TRUE if a match is found
	 */
	bool (*equals_subjectAltName) (const x509_t *this, identification_t *id);

	/**
	 * @brief Checks the validity interval of the certificate
	 * 
	 * @param this			certificate being examined
	 * @param until			until = min(until, notAfter)
	 * @return			NULL if the certificate is valid
	 */
	err_t (*is_valid) (const x509_t *this, time_t *until);
	
	/**
	 * @brief Returns the CA basic constraints flag
	 * 
	 * @param this			certificate being examined
	 * @return			TRUE if the CA flag is set
	 */
	bool (*is_ca) (const x509_t *this);
	
	/**
	 * @brief Destroys the certificate.
	 * 
	 * @param this			certificate to destroy
	 */
	void (*destroy) (x509_t *this);

	/**
	 * @brief Log x509 certificate info.
	 *
	 * @param this			certificate to log
	 * @param logger		logger to be used
	 * @param utc			log dates either in UTC or local time
	 * @param has_key		a matching private key is available
	 */
	 void (*log_certificate) (const x509_t *this, logger_t *logger, bool utc, bool has_key);
};

/**
 * @brief Read a x509 certificate from a DER encoded blob.
 * 
 * @param chunk 	chunk containing DER encoded data
 * @return 			created x509_t certificate, or NULL if invalid.
 * 
 * @ingroup transforms
 */
x509_t *x509_create_from_chunk(chunk_t chunk);

/**
 * @brief Read a x509 certificate from a DER encoded file.
 * 
 * @param filename 	file containing DER encoded data
 * @param label		label describing kind of certificate
 * @return 			created x509_t certificate, or NULL if invalid.
 * 
 * @ingroup transforms
 */
x509_t *x509_create_from_file(const char *filename, const char *label);

#endif /* X509_H_ */

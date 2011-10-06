/* Texture Implementation - Model Branch
 *
 * Texture.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCoordinator.h"    // for the Coordinator Interface
#include "iAPITexture.h"     // for the APITexture Interface
#include "iUtilities.h"      // for error(), nameWithDir()

#include "Texture.h"         // for Texture class definition
#include "Configuration.h"   // for TEXTURE_DIRECTORY

//-------------------------------- Texture ------------------------------------
//
// The Texture class defines the structure for a texture in the Model Branch
//
// The Coordinator manages the Texture Instances
//
iCoordinator* Texture::coordinator = NULL;

// CreateTexture creates a Texture object
//
iTexture* CreateTexture(const wchar_t* file) {

	return new Texture(file);
}

iTexture* Clone(const iTexture* src) {

    return src->clone();
}

// constructor adds a pointer to the texture to the coordinator, creates the full
// name of the texture file, stores the name and creates the texture's api
// representation
//
Texture::Texture(const wchar_t* file) {

	if (!coordinator)
		error(L"Texture 00: Couldn\'t access the Coordinator");
	else if (!coordinator->add(this))
		error(L"Texture 01: Couldn\'t add the texture to the Coordinator");

	apiTexture = CreateAPITexture(file);
}

// copy constructor initializes the instance pointers and calls the
// assignment operator
//
Texture::Texture(const Texture& src) {

	if (!coordinator)
		error(L"Texture 00: Couldn\'t access the Coordinator");
	else if (!coordinator->add(this))
		error(L"Texture 01: Couldn\'t add the texture to the Coordinator");
	
	apiTexture = NULL;
	*this      = src;
}

// assignment operator discards the old data and copies new data
// from src - does not copy the APITexture from src but creates
// a new APITexture instead
//
Texture& Texture::operator=(const Texture& src) {

	if (this != &src) {
		if (apiTexture)
			apiTexture->Delete();
        if (src.apiTexture)
            apiTexture = src.apiTexture->clone();
        else
            apiTexture = NULL;
	}

	return *this;
}

// attach attaches the apiTexture to the pipeline
//
void Texture::attach(unsigned flags) const {

	if (apiTexture) {  
		apiTexture->attach(flags);
    }
}

// detach detaches the apiTexture from the pipeline
//
void Texture::detach() {

	if (apiTexture)
		apiTexture->detach();
}

// draw draws the apiTexture 
//
void Texture::draw(const RelRect& hudPos, unsigned char a) 
 const {

	if (apiTexture)
		apiTexture->draw(hudPos, a);
}

// suspend suspends the device texture 
//
void Texture::suspend() {

	if (apiTexture)
		apiTexture->suspend();
}

// release releases the device texture 
//
void Texture::release() {

	if (apiTexture)
		apiTexture->release();
}

// destructor deletes the graphics card representation and removes the 
// pointer to the texture from the coordinator
//
Texture::~Texture() {

	apiTexture->Delete();

    if (coordinator)
		coordinator->remove(this);
	else
		error(L"Texture 90:: Couldn\'t access the Coordinator");
}

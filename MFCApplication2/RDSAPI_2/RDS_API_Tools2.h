#pragma once

#ifndef uchar
typedef unsigned char uchar;
#endif

#ifndef ulong
typedef unsigned long ulong;
#endif


/// This class is based on work D3DES (V5.09) - A portable, public domain, version of the Data Encryption Standard.
/// Written with Symantec's THINK (Lightspeed) C by Richard Outerbridge.
///
class cDES
{
public:
	cDES(void);
	~cDES();

	enum class eDESMode
	{
		DESM_ENCRYPTION = 0,
		DESM_DECRYPTION = 1,
	};

	/// Sets the internal key register according to the hexadecimal
	/// key contained in the 8 bytes of hexkey, according to the DES,
	/// for encryption or decryption according to Mode.
	/// \param ucKey	len=8
	/// \param Mode
	void SetKey(uchar* ucKey, eDESMode Mode);

	/// Encrypts/Decrypts (according to the key currently loaded in the
	/// internal key register) one block of eight bytes at address 'from'
	/// into the block at address 'to'.  They can be the same.
	/// \param ucFrom	len=8
	/// \param ucTo		len=8
	void Compute(uchar* ucFrom, uchar* ucTo);

protected:
	ulong KnL[32];

	void scrunch(uchar*, ulong*);
	void unscrun(ulong*, uchar*);
	void desfunc(ulong*, ulong*);
	void cookey(ulong*);

	/*                  cookedkey[32]
	* Loads the internal key register with the data in cookedkey.
	*/
	void usekey(register ulong* from);

	/*                 cookedkey[32]
	* Copies the contents of the internal key register into the storage
	* located at &cookedkey[0].
	*/
	void cpkey(register ulong* into);
};
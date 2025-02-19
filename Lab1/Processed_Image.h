
#ifndef PROCESSED_IMAGE_H

#define PROCESSED_IMAGE_H
/// <summary>
/// Struct - envelope for the processed image
/// </summary>
/// <typeparam name="TImage">Type of the image</typeparam>
template<class TImage>
struct Processed_Image
{
	typedef void (*destr_func)(TImage);//Func pointer that points at function-destructor

	/// <summary>
	/// Main ctor
	/// </summary>	
	/// <param name="block">Name fo the processing block</param>
	/// <param name="img">Pointer to the image type</param>
	/// <param name="destrFunc">Function that will clear the image</param>
	Processed_Image(const char* block, TImage img, destr_func destrFunc)
	{
		m_img = img;
		m_block = block;		
		m_dest_function = destrFunc;
	}
	/// <summary>
	/// Copy ctor
	/// </summary>
	/// <param name="other"></param>
	Processed_Image(const Processed_Image<TImage>& other)
	{
		m_img = other.m_img;
		m_block = other.m_block;
		m_dest_function = other.m_dest_function;
	}

	/// <summary>
	/// Assignment operator
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	Processed_Image<TImage>& operator = (const Processed_Image<TImage>& other)
	{
		m_img = other.m_img;
		m_block = other.m_block;
		m_dest_function = other.m_dest_function;

		return *this;
	}

	/// <summary>
	/// Destructor
	/// </summary>
	~Processed_Image()
	{
		if (m_dest_function != nullptr)
			m_dest_function(m_img);
	}	
	/// <summary>
	/// Get the block name
	/// </summary>
	/// <returns></returns>
	const char* getBlock()
	{
		return m_block;
	}
	
	/// <summary>
	/// Get the image
	/// </summary>
	/// <returns>Returns the image</returns>
	TImage getImage()
	{
		return m_img;
	}

private:
	TImage m_img;
	const char* m_block;
	destr_func m_dest_function;
};

#endif


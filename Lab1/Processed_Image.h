
#ifndef PROCESSED_IMAGE_H

#include<string>

#define PROCESSED_IMAGE_H
/// <summary>
/// Struct - envelope for the processed image
/// </summary>
/// <typeparam name="TImage">Type of the image</typeparam>
template<class TImage>
struct Processed_Image
{
	typedef void (*destr_func)(TImage);//Func pointer that points at function-destructor
	typedef TImage(*copy_func)(TImage);

	Processed_Image() 
	{}

	/// <summary>
	/// Main ctor
	/// </summary>	
	/// <param name="block">Name fo the processing block</param>
	/// <param name="img">Pointer to the image type</param>
	/// <param name="destrFunc">Function that will clear the image</param>
	Processed_Image(const std::string& block, TImage img, destr_func destrFunc, copy_func copy_function)
	{
		if (copy_function == nullptr)
			throw std::exception("Copy_Function was not set!");

		m_img = img;
		m_block = block;
		m_dest_function = destrFunc;
		m_copy_function = copy_function;

#ifndef NDEBUG
		//OutputDebugStringA(this);
		OutputDebugStringA("\n");
		OutputDebugStringA("Main Ctor");
#endif
	}
	/// <summary>
	/// Copy ctor
	/// </summary>
	/// <param name="other"></param>
	Processed_Image(const Processed_Image<TImage>& other)
	{
		m_copy_function = other.m_copy_function;
		m_img = m_copy_function(other.m_img);
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
		m_copy_function = other.m_copy_function;
		m_img = m_copy_function(other.m_img);
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
		
#ifndef NDEBUG
		//OutputDebugStringA(this);
		OutputDebugStringA("\n");
		OutputDebugStringA("Destructor Call");
#endif
	}	
	/// <summary>
	/// Get the block name
	/// </summary>
	/// <returns></returns>
	std::string& getBlock()
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
	std::string m_block;
	destr_func m_dest_function;
	copy_func m_copy_function;	
};

#endif


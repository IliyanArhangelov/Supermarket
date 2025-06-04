#pragma once

class ProductCategory
{
	size_t id;
	const char* name;
	const char* description;

	void free();
	void copyFrom(const ProductCategory& other);
	void moveFrom(ProductCategory&& other) noexcept;
public:
	ProductCategory() = delete;
	ProductCategory(size_t id,  const char* name, const char* description);
	ProductCategory(const ProductCategory& other);
	ProductCategory(ProductCategory&& other) noexcept;
	~ProductCategory();

	ProductCategory& operator=(const ProductCategory& ohter);
	ProductCategory& operator=(ProductCategory&& other) noexcept;
};


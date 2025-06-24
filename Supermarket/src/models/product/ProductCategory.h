#pragma once
#include <iostream>
class ProductCategory
{
	size_t id;
	char* name;
	char* description;

	void free();
	void copyFrom(const ProductCategory& other);
	void moveFrom(ProductCategory&& other) noexcept;
public:
	ProductCategory() = delete;
	ProductCategory(size_t id,  const char* name, const char* description);
	ProductCategory(const ProductCategory& other);
	ProductCategory(ProductCategory&& other) noexcept;
	~ProductCategory();

	ProductCategory& operator=(const ProductCategory& other);
	ProductCategory& operator=(ProductCategory&& other) noexcept;

	static const ProductCategory* findById(size_t id, ProductCategory** allCategories, size_t count);
	static const ProductCategory* findByName(const char* name, ProductCategory** allCategories, size_t count);

	void serialize(std::ostream& os) const;
	size_t getId() const;
	const char* getName() const;
	const char* getDescription() const;
};


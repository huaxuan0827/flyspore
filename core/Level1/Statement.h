#pragma once
#include <memory>
#include <functional>
#include <string>
namespace fs
{
	namespace L1
	{
		using IdType = unsigned long long;

		class AnyValues;
		using P_AnyValues = std::shared_ptr<AnyValues>;
		class PropertyNode;
		using P_PropertyNode = std::shared_ptr<PropertyNode>;
		using PW_PropertyNode = std::weak_ptr<PropertyNode>;


		class Data;
		using P_Data = std::shared_ptr<Data>;

		class DataPack;
		using P_DataPack = std::shared_ptr<DataPack>;

		class DataAdapter;
		using P_DataAdapter = std::shared_ptr<DataAdapter>;

		class Spore;
		using P_Spore = std::shared_ptr<Spore>;
		using PW_Spore = std::weak_ptr<Spore>;

		class Path;
		using P_Path = std::shared_ptr<Path>;

		class Pin;
		using P_Pin = std::shared_ptr<Pin>;

		enum class Pin_Type : unsigned int
		{
			UNKNOW = 0,
			IN_PIN,
			OUT_PIN
		};

		class Path;
		using P_Path = std::shared_ptr<Path>;

		class Session;
		using P_Session = std::shared_ptr<Session>;

		class Context;

		using Pin_Process = std::function< void(Context& cc, P_Data data)>;
	}
}
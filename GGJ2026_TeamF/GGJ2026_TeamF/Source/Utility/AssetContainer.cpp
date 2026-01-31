#include "AssetContainer.h"
#include "DxLib.h"
#include <mutex>

const std::vector<int>& AssetContainer::GetImages(const std::string& file_name, int div, int dx, int dy, int px, int py)
{
	// 連想配列内に取得したいハンドル情報があるか確認する（多重読み込み禁止）
	size_t find_data = images_container.count(file_name);

	// 連想配列内になければ、ハンドル情報を読み込む
	if (find_data == NULL)
	{
		// 分割読み込みする必要があるか確認する
		if (div)
		{
			// 分割読み込みする
			CreateSprite(file_name, div, dx, dy, px, py);
		}
		else
		{
			// 分割読み込みしない
			CreateSprite(file_name);
		}
	}

	// ハンドル情報を返却する
	return images_container.at(file_name);
}

int AssetContainer::GetSound(const std::string& file_name)
{
	// 連想配列内に取得したいハンドル情報があるか確認する（多重読み込み禁止）
	size_t find_data = images_container.count(file_name);

	// 連想配列内になければ、ハンドル情報を読み込む
	if (find_data == NULL)
	{
		CreateSound(file_name);
	}

	// ハンドル情報を返却する
	return sounds_container.at(file_name);
}

void AssetContainer::AllRelease()
{
	// 画像ハンドル情報の解放
	ReleaseSprite();

	// 音源ハンドル情報の解放
	ReleaseSound();
}

void AssetContainer::CreateSprite(const std::string& file_name, int div, int dx, int dy, int px, int py)
{
	// 画像が格納されているフォルダパスの指定
	std::string file_path = "Resource/Sprites/";
	// 引数とフォルダパスをくっ付ける
	file_path += file_name;
	// 分割読み込みする量だけメモリを動的にメモリを確保する
	std::unique_ptr<int[]> handle(new int[div]);

	// 画像の分割読み込みを行う
	int result = LoadDivGraph(file_path.c_str(), div, dx, dy, px, py, handle.get());

	// 戻り値に異常があれば、通知する
	if (result == -1)
	{
		throw std::runtime_error("画像が読み込めませんでした");
	}

	// 連想配列内に格納する
	for (int i = 0; i < div; i++)
	{
		images_container[file_name].emplace_back(handle[i]);
	}
}

void AssetContainer::CreateSprite(const std::string& file_name)
{
	// 画像が格納されているフォルダパスの指定
	std::string file_path = "Resource/Sprites/";
	// 引数とフォルダパスをくっ付ける
	file_path += file_name;

	// 画像の読み込み
	int handle = LoadGraph(file_path.c_str());
	// 戻り値に異常があれば、通知する
	if (handle == -1)
	{
		throw std::runtime_error("画像が読み込めませんでした");
	}

	// 連想配列内に格納する
	images_container[file_name].emplace_back(handle);
}

void AssetContainer::CreateSound(const std::string& file_name)
{
	// 音源が格納されているフォルダパスの指定
	std::string file_path = "Resource/Sounds/";
	// 引数とフォルダパスをくっ付ける
	file_path += file_name;

	// 画像の読み込み
	int handle = LoadSoundMem(file_path.c_str());
	// 戻り値に異常があれば、通知する
	if (handle == -1)
	{
		throw std::runtime_error("音源が読み込めませんでした");
	}

	// 連想配列内に格納する
	sounds_container.emplace(file_name, handle);
}

void AssetContainer::ReleaseSprite()
{
	// 連想配列が空なら、処理を終了する
	if (images_container.empty())
	{
		return;
	}

	// 連想配列内の要素を解放していく
	for (std::pair<std::string, std::vector<int>> data : images_container)
	{
		// 画像ハンドル情報の解放
		DeleteSharingGraph(data.second[0]);
		// ベクター配列の解放
		data.second.clear();
	}
	// 連想配列を解放する
	images_container.clear();
}

void AssetContainer::ReleaseSound()
{
	// 連想配列が空なら、処理を終了する
	if (sounds_container.empty())
	{
		return;
	}

	// 連想配列内の要素を解放していく
	for (std::pair<std::string, int> data : sounds_container)
	{
		// 音源ハンドル情報の解放
		DeleteSoundMem(data.second);
	}
	// 連想配列を解放する
	sounds_container.clear();
}

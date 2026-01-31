#pragma once

#include "Singleton.h"
#include <map>
#include <string>
#include <vector>

// ゲームに使用する素材を管理するクラス
class AssetContainer final : public Singleton<AssetContainer>
{
private:
	// 画像を管理する連想配列
	std::map<std::string, std::vector<int>> images_container;

	// 音源を管理する連想配列
	std::map<std::string, int> sounds_container;

public:
	/// <summary>
	/// 画像ハンドル情報の取得
	/// </summary>
	/// <param name="file_name">画像が格納されているフォルダパス</param>
	/// <param name="div">総分割数</param>
	/// <param name="dx">横方向の分割数</param>
	/// <param name="dy">縦方向の分割数</param>
	/// <param name="px">横方向に分割したときのピクセル数</param>
	/// <param name="py">縦方向に分割したときのピクセル数</param>
	/// <returns>画像ハンドル配列</returns>
	const std::vector<int>& GetImages(const std::string& file_name, int div = 0, int dx = 0, int dy = 0, int px = 0, int py = 0);
	
	/// <summary>
	/// 音源ハンドル情報の取得
	/// </summary>
	/// <param name="file_name">音源が格納されているフォルダパス</param>
	/// <returns>音源ハンドル</returns>
	int GetSound(const std::string& file_name);

	/// <summary>
	/// 保持しているハンドル情報の解放
	/// </summary>
	void AllRelease();

private:
	// 画像ハンドル情報の読み込み（分割読み込み）
	void CreateSprite(const std::string& file_name, int div, int dx, int dy, int px, int py);
	// 画像ハンドル情報の読み込み
	void CreateSprite(const std::string& file_name);
	// 音源ハンドル情報の読み込み
	void CreateSound(const std::string& file_name);

	// 取得した画像ハンドル情報の解放
	void ReleaseSprite();
	// 取得した音源ハンドル情報の解放
	void ReleaseSound();

};

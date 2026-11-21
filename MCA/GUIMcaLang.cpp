#include <sstream>
#include <vector>

#include "GUIMcaLang.h"
#include "helpers.h"

CGUIMcaLang::CGUIMcaLang(void)
{
	for (std::map<const std::string, const std::string>::const_iterator i = m_default_lang.begin(); i != m_default_lang.end(); i++)
	{
		m_curr_lang.insert(*i);
	}
}

void CGUIMcaLang::trimLeft(std::string& str, const char* chars2remove)
{
	if (!str.empty())
	{
		std::string::size_type pos = str.find_first_not_of(chars2remove);

		if (pos != std::string::npos)
			str.erase(0,pos);
		else
			str.erase( str.begin() , str.end() );
	}
}

void CGUIMcaLang::trimRight(std::string& str, const char* chars2remove)
{
	if (!str.empty())
	{
		std::string::size_type pos = str.find_last_not_of(chars2remove);

		if (pos != std::string::npos)
			str.erase(pos+1);
		else
			str.erase( str.begin() , str.end() );
	}
}

void CGUIMcaLang::initLang(std::string input)
{
	replace(&input, "\r\n", "\n");
	replace(&input, "\r", "\n");
	std::istringstream instream(input);
	std::string line;
	std::vector<std::string> all_lines;

	while(std::getline(instream, line, '\n'))
	{
		trimLeft(line, " ");
		trimRight(line, " ");
		if (line.empty()) continue;
		if (line[0] == '#' || (line[0] == '/' && line[1] == '/')) continue;
		if (line.find("=") == std::string::npos) continue;

		all_lines.push_back(line);
		//sio_printf("%s\n", line.c_str());
	}

	std::string::size_type subpos;
	std::string::size_type laspos;
	for (std::map<const std::string, const std::string>::const_iterator i = m_default_lang.begin(); i != m_default_lang.end(); i++)
	{
		for (u32 li = 0; li < all_lines.size(); li++)
		{
			subpos = all_lines[li].find((*i).first);
			if (subpos != std::string::npos)
			{
				if (all_lines[li][subpos+(*i).first.length()] == ' ' || all_lines[li][subpos+(*i).first.length()] == '=')
				{
					subpos = all_lines[li].find("\"");
					laspos = all_lines[li].rfind("\"");
					if (subpos != laspos)
					{
						m_curr_lang[(*i).first] = all_lines[li].substr(subpos+1, (laspos)-(subpos+1));
						replace(&m_curr_lang[(*i).first], "\\\"", "\"");
						replace(&m_curr_lang[(*i).first], "\\n", "\n");
					}
				}
			}
		}
	}

	all_lines.clear();
}

CGUIMcaLang::~CGUIMcaLang(void)
{
	m_curr_lang.clear();
}

const char *CGUIMcaLang::getText(const char *label)
{
	return m_curr_lang[label].c_str();
}

void CGUIMcaLang::replace(std::string *base, const std::string find, const std::string repl)
{
	int lenrepl = repl.length();
	int lenfind = find.length();

	u32 i = 0;
	while ( std::string::npos != (i = base->find(find, i)) )
	{
		base->replace( i, lenfind, repl);
		i += lenrepl;
	}
}

void CGUIMcaLang::replace(std::string *base, const std::string find, const long long repln)
{
	std::ostringstream conv;

	conv << repln;
	std::string repl = conv.str();

	int lenrepl = repl.length();
	int lenfind = find.length();

	u32 i = 0;
	while ( std::string::npos != (i = base->find(find, i)) )
	{
		base->replace( i, lenfind, repl);
		i += lenrepl;
	}
}

const CGUIMcaLang::langmapRaw CGUIMcaLang::m_default_lang_raw[] = {
   CGUIMcaLang::langmapRaw("LNG_MWND_CHOOSE_SLOT", "选择插槽"),
   CGUIMcaLang::langmapRaw("LNG_OPER_PS2_MEMORY_CARD", "PS2 记忆卡"),
   CGUIMcaLang::langmapRaw("LNG_OPER_PSX_MEMORY_CARD", "PS1 记忆卡"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FORMAT", "格式化"),
   CGUIMcaLang::langmapRaw("LNG_OPER_UNFORMAT", "取消格式化"),
   CGUIMcaLang::langmapRaw("LNG_OPER_MAKE_IMAGE", "创建记忆卡镜像"),
   CGUIMcaLang::langmapRaw("LNG_OPER_RESTORE_IMAGE", "还原记忆卡镜像"),
   CGUIMcaLang::langmapRaw("LNG_OPER_INFO", "记忆卡信息"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FAST", "快速"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FULL", "完整"),
   CGUIMcaLang::langmapRaw("LNG_OPER_CHOOSE_MC_SIZE", "\n请选择记忆卡容量"),
   CGUIMcaLang::langmapRaw("LNG_OPER_MEGABYTE", "{CARDSIZE} MB"),
   CGUIMcaLang::langmapRaw("LNG_OPER_YES", "是"),
   CGUIMcaLang::langmapRaw("LNG_OPER_NO", "否"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_FORMAT_DATA_LOST", "确定要格式化这张记忆卡吗？\n\n格式化将清除卡上存储的所有数据！"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_UNFORMAT_DATA_LOST", "确定要取消这张记忆卡的格式化吗？\n\n取消格式化将清除卡上存储的所有数据！"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_RESTORE_DATA_LOST", "确定要还原这张记忆卡镜像吗？\n\n还原镜像将清除卡上存储的所有数据！"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_OVERWRITE", "所选文件已存在！确定要覆盖它吗？\n\n覆盖将导致文件内容全部丢失。"),
   CGUIMcaLang::langmapRaw("LNG_WARN_CAP", "警告……"),
   CGUIMcaLang::langmapRaw("LNG_WARN_NO_CARD_IN_SLOT", "插槽 {SLOTNUM} 中的记忆卡已被拔出或更换。程序将返回主菜单。\n\n\n按 X 键继续。"),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_DO_NOT_REMOVE", "\n请勿拔出记忆卡、复位或关闭主机电源"),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_SUCCESS", "\n操作成功完成\n按 X 键继续"),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_FAIL", "\n操作失败\n按 X 键继续"),
   CGUIMcaLang::langmapRaw("LNG_FILE_CHOOSE_FILE_READ", "选择要读取的文件"),
   CGUIMcaLang::langmapRaw("LNG_FILE_CHOOSE_FILE_SAVE", "选择要写入的文件"),
   CGUIMcaLang::langmapRaw("LNG_INFO_CAPTION", "记忆卡信息"),
   CGUIMcaLang::langmapRaw("LNG_INFO_UNIT_MB", "MB"),
   CGUIMcaLang::langmapRaw("LNG_INFO_UNIT_KB", "kB"),
   CGUIMcaLang::langmapRaw("LNG_INFO_MC_PSX", "PS1"),
   CGUIMcaLang::langmapRaw("LNG_INFO_MC_PS2", "PS2"),
   CGUIMcaLang::langmapRaw("LNG_INFO_SLOT", "记忆卡插槽："),
   CGUIMcaLang::langmapRaw("LNG_INFO_TYPE", "卡片类型："),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESIZE", "页大小："),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESINBLOCK", "每块页数："),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESTOTAL", "总页数："),
   CGUIMcaLang::langmapRaw("LNG_INFO_THX_TXT", "\n感谢 Sony 造出这台主机，它陪伴我们度过了许多个漫长的夜晚，让我们无数次熬到深夜，还害我们的女朋友和我们分手 ;)。\n\n感谢 PiotrB 提供他的 PS1 记忆卡照片，以及所有参与 PS1 记忆卡照片拍摄的人。"),
   CGUIMcaLang::langmapRaw("LNG_INFO_THX_CAP", "鸣谢："),
   CGUIMcaLang::langmapRaw("LNG_TIP_VKBD_MASK", "START - 新建文件\nL2/R2 - 启用/禁用文件掩码"),
   CGUIMcaLang::langmapRaw("LNG_TIP_MASK_ENABLED", "文件掩码已启用"),
   CGUIMcaLang::langmapRaw("LNG_TIP_MASK_DISABLED", "文件掩码已禁用"),
   CGUIMcaLang::langmapRaw("LNG_VKBD_WARN_WRONG_NAME", "指定的文件名包含无效字符。请更改文件名后重试。\n\n\n按任意键继续。"),
   CGUIMcaLang::langmapRaw("LNG_EXIT_SELECT_ELF", "\n\n请选择要执行的文件……"),
   CGUIMcaLang::langmapRaw("LNG_EXIT_INVALID", "\n\n所选文件不是有效的 ELF 可执行文件。"),
   CGUIMcaLang::langmapRaw("LNG_EXIT_FAILED", "\n\n无法打开所选文件。"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_SIZE_MISMATCH", "所选容量大于默认值，这可能损坏记忆卡！\n\n确定要继续吗？"),
};
const int CGUIMcaLang::m_numof_entries = countof(CGUIMcaLang::m_default_lang_raw);
const std::map<const std::string, const std::string> CGUIMcaLang::m_default_lang(CGUIMcaLang::m_default_lang_raw, CGUIMcaLang::m_default_lang_raw + CGUIMcaLang::m_numof_entries);

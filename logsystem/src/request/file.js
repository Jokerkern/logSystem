import { get } from './http'

// 左侧菜单选择的为 全部 时，根据路径，获取文件列表
export const getFileListByPath = (p) => get('/file/getfilelist', p)
// 左侧菜单选择的为 除全部以外 的类型时，根据类型，获取文件列表
export const getFileListByType = (p) => get('/file/selectfilebyfiletype', p)
import { get} from './http'

// 登录接口
export const login = (p) => get('/api/v1/todo/total', p)

// 注册接口
export const Query = (p) => get('/api/v1/todo/', p)

// 获取登录状态及用户信息
export const checkUserLoginInfo = (p) => get('/user/checkuserlogininfo', p)
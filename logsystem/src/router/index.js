import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../views/Home.vue'
import About from '../views/About.vue'
import Setting from '../views/Setting.vue'
import Logorm from '../views/Logorm.vue'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'Aboud',
    component:About
  },
  {
    path: '/About',
    name: 'About',
    component: About,
    
  },
  {
    path: '/Home',
    name: 'Home',
    component: Home,
  },
  {
    path: '/Logorm',
    name: 'Logorm',
    component: Logorm,
  },
  {
    path: '/Setting',
    name: 'Setting',
    component: Setting,
  },
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router

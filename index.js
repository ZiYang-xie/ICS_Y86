import Main from './pages/main.js'
import Info from './pages/info.js'
import About from './pages/about.js'

let app = new Vue({
    el: '#app',
    data: {
        banner_show: true,
        banner_class: "fade_out"
    },
    mounted() {
        this.timer = setTimeout(()=>{   //设置延迟执行
            this.banner_show = false;
        },2300);
    },
    router: new VueRouter({
        mode: "history",
        routes:[
            {
                path:"/",
                redirect:'/main'
            },
            {
                path:"/main",
                component: Main
            },
            {
                path:"/info",
                component: Info
            },
            {
                path:"/about",
                component: About
            }
        ]
    })
})

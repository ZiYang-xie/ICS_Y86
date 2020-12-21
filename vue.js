import Main from './pages/main.js'
import Extended from './pages/extended.js'
import About from './pages/about.js'

let app = new Vue({
    el: '#app',
    data: {
        text: ""
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
                path:"/extended",
                component: Extended
            },
            {
                path:"/about",
                component: About
            }
        ]
    })
})

export default app;
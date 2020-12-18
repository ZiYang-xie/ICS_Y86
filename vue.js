import Main from './pages/main.js'
import Extended from './pages/extended.js'

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
            }
        ]
    })
})

export default app;
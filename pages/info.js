import Main from "./main.js"

export default {
    data: function (){
        return {
            terminal: ""
        }
    },
    created: function() {
        this.text = Main.data();
    },
    template:`
    <Content :style="{padding: '0 50px'}">
        <div class="topInfo_style">
            
        </div>
        
        <Card shadow dis-hover="true">
            <h1> To be continue </h1>
        </Card>
    </Content>
    `
}
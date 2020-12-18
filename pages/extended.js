import Main from "./main.js"

export default {
    data: function (){
        return {
        }
    },
    created: function() {
        this.text = Main.data().task_name;
        console.log(this.text)
    },
    template:`
    <Content :style="{padding: '0 50px'}">
        <div class="topInfo_style">
            <p>扩展信息 / </p>
            <p>断点添加 / </p>
            <p>终端输出 / </p>
            <p>彩色矩阵输出 / </p>
            <p>{{text}} / </p>
        </div>
        
        <Card shadow dis-hover="true">
            <div class="left_card_wrapper">
                <Card class="text_card">
                    <List>
                        <ListItem>
                            <ListItemMeta :title=text />
                            <template slot="action">
                                <li>
                                    <a href="">Edit</a>
                                </li>
                                <li>
                                    <a href="">More</a>
                                </li>
                            </template>
                        </ListItem>
                    </List>
                </Card>
            </div>
        </Card>
    </Content>
    `
}
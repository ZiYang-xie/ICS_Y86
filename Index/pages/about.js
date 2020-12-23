export default {
    data: function (){
        return {

        }
    },
    template:`
    <Content :style="{padding: '0 50px'}">
        <div class="topInfo_style">
            
        </div>
        
        <Card shadow dis-hover="true">
            <div class="about_wapper">
                <h1 class="about_title">关于 - About</h1>
                <Divider><Icon type="md-copy" size="20" /> 项目信息</Divider>
                    <div class="about_passage">
                        <p>该项目是复旦大学2020学年计算机系统基础 (ICS) 拔尖班的期末PJ</p>
                        <p>本项目在基础流水线处理器架构之上添加了 <b>y86 指令集扩展</b>，并编写了一个<b>汇编器</b> (支持宏定义)，具体介绍请看说明文档。</p>
                        <p>结合精美的前端，根据上传文件性质不同 (yo/ys) 可以在普通模式和编译模式下运行程序, 能够直观的看到基本信息、所有的寄存器信息 (包括流水线寄存器)、运行进度、当前指令和其他信息。</p>
                        <b>支持任意前进回溯、设置断点、终端输出 (包含彩色矩阵)等扩展功能</b>
                    </div>
                <Divider><Icon type="md-contact" size="20" /> 作者信息</Divider>
                <div class="about_author">
                    <p class="author"><Icon type="md-cog" size="24" /> <b>后端: </b> 王少文巨佬</p>
                    <p class="author"><Icon type="logo-css3" size="24" /> <b>前端及测试: </b> 谢子飏蒟蒻</p>
                </div>
                <Divider><Icon type="logo-github" size="20" /> github 地址</Divider>
                <a class="about_link" href="https://github.com/fdu2019xzy/ICS_Y86">https://github.com/fdu2019xzy/ICS_Y86</a>
            </div>
        </Card>
    </Content>
    `
}
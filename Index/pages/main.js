export default {
    data: function (){
        return {
            task_name:"None",
            file_path: "../static/source/task.yo",
            file_format: ["yo","ys"],
            upload_list: [],
            reg_list1: [
                {
                    title: "RAX",
                    key: "rax",
                    align: 'center'
                },
                {
                    title: "RCX",
                    key: "rcx",
                    align: 'center'
                },
                {
                    title: "RDX",
                    key: "rdx",
                    align: 'center'
                },
                {
                    title: "RBX",
                    key: "rbx",
                    align: 'center'
                },
                {
                    title: "RSP",
                    key: "rsp",
                    align: 'center'
                }
            ],
            reg_list2: [
                {
                    title: "RBP",
                    key: "rbp",
                    align: 'center'
                },
                {
                    title: "RSI",
                    key: "rsi",
                    align: 'center'
                },
                {
                    title: "RDI",
                    key: "rdi",
                    align: 'center'
                },
                {
                    title: "R8",
                    key: "r8",
                    align: 'center'
                },
                {
                    title: "R9",
                    key: "r9",
                    align: 'center'
                }
            ],
            reg_list3: [
                {
                    title: "R10",
                    key: "r10",
                    align: 'center'
                },
                {
                    title: "R11",
                    key: "r11",
                    align: 'center'
                },
                {
                    title: "R12",
                    key: "r12",
                    align: 'center'
                },
                {
                    title: "R13",
                    key: "r13",
                    align: 'center'
                },
                {
                    title: "R14",
                    key: "r14",
                    align: 'center'
                }
            ],
            reg_data1: [
                {
                    rax: "0x0",
                    rcx: "0x0",
                    rdx: "0x0",
                    rbx: "0x0",
                    rsp: "0x0"
                }
            ],
            reg_data2: [
                {
                    rbp: "0x0",
                    rsi: "0x0",
                    rdi: "0x0",
                    r8: "0x0",
                    r9: "0x0"
                }
            ],
            reg_data3: [
                {
                    r10: "0x0",
                    r11: "0x0",
                    r12: "0x0",
                    r13: "0x0",
                    r14: "0x0",
                    cellClassName: {
                        r10: 'demo-table-info-cell-name'
                    }
                }
            ],
            info_list: [
                {
                    title: "状态",
                    key: "State",
                    align: 'center'
                },
                {
                    title: "PC",
                    key: "PC",
                    align: 'center'
                },
            ],
            info_data: [
                {
                    State: "AOK",
                    PC: "0x000"
                }
            ],
            cc_list: [
                {
                    title: "条件标志",
                    key: "cc",
                    align: 'center'
                },
                {
                    title: "ZF",
                    key: "zf",
                    align: 'center'
                },
                {
                    title: "SF",
                    key: "sf",
                    align: 'center'
                },
                {
                    title: "OF",
                    key: "of",
                    align: 'center'
                }
            ],
            cc_data: [
                {
                    zf: "1",
                    sf: "0",
                    of: "0"
                }
            ],
            pipline_reg_list: {
                f_reg_list: [
                    {
                        title: "Fetch阶段",
                        key: "name"
                    },
                    {
                        title: "PredPc",
                        key: "predPC"
                    }
                ],
                d_reg_list: [
                    {
                        title: "Decode阶段",
                        key: "name"
                    },
                    {
                        title: "Stat",
                        key: "Stat"
                    },
                    {
                        title: "Icode",
                        key: "icode"
                    },
                    {
                        title: "Ifun",
                        key: "ifun"
                    },
                    {
                        title: "RA",
                        key: "rA"
                    },
                    {
                        title: "RB",
                        key: "rB"
                    },
                    {
                        title: "valC",
                        key: "valC"
                    },
                    {
                        title: "valP",
                        key: "valP"
                    }
                ],
                e_reg_list: [
                    {
                        title: "Execute阶段",
                        key: "name"
                    },
                    {
                        title: "Stat",
                        key: "Stat"
                    },
                    {
                        title: "Icode",
                        key: "icode"
                    },
                    {
                        title: "Ifun",
                        key: "ifun"
                    },
                    {
                        title: "valC",
                        key: "valC"
                    },
                    {
                        title: "valA",
                        key: "valA"
                    },
                    {
                        title: "valB",
                        key: "valB"
                    },
                    {
                        title: "srcA",
                        key: "srcA"
                    },
                    {
                        title: "srcB",
                        key: "srcB"
                    },
                    {
                        title: "dstE",
                        key: "dstE"
                    },
                    {
                        title: "dstM",
                        key: "dstM"
                    }
                ],
                m_reg_list: [
                    {
                        title: "Memory阶段",
                        key: "name"
                    },
                    {
                        title: "Stat",
                        key: "Stat"
                    },
                    {
                        title: "Icode",
                        key: "icode"
                    },
                    {
                        title: "valE",
                        key: "valE"
                    },
                    {
                        title: "valA",
                        key: "valA"
                    },
                    {
                        title: "dstE",
                        key: "dstE"
                    },
                    {
                        title: "dstM",
                        key: "dstM"
                    }
                ],
                w_reg_list: [
                    {
                        title: "WriteBack阶段",
                        key: "name"
                    },
                    {
                        title: "Stat",
                        key: "Stat"
                    },
                    {
                        title: "Icode",
                        key: "icode"
                    },
                    {
                        title: "valE",
                        key: "valE"
                    },
                    {
                        title: "valM",
                        key: "valM"
                    },
                    {
                        title: "dstE",
                        key: "dstE"
                    },
                    {
                        title: "dstM",
                        key: "dstM"
                    }
                ]
            },
            pipline_reg_data: {
                f_reg_data: [
                    {
                        name: "F_REG",
                        predPC: "0x000"
                    }
                ],
                d_reg_data: [
                    {
                        name: "D_REG",
                        Stat: "AOK",
                        icode: "0",
                        ifun : "0",
                        rA: "None",
                        rB: "None",
                        valC: "0x00",
                        valP: "0x00",
                    }
                ],
                e_reg_data: [
                    {
                        name: "E_REG",
                        Stat: "AOK",
                        icode: "0",
                        ifun: "0",
                        valC: "0x0",
                        valA: "0x00",
                        valB: "0x00",
                        srcA: "None",
                        srcB: "None",
                        dstE: "None",
                        detM: "None"
                    }
                ],
                m_reg_data: [
                    {
                        name: "M_REG",
                        Stat: "AOK",
                        icode: "0",
                        valE: "0x00",
                        valA: "0x00",
                        dstE: "None",
                        detM: "None"
                    }
                ],
                w_reg_data: [
                    {
                        name: "W_REG",
                        Stat: "AOK",
                        icode: "0",
                        valE: "0x0",
                        valM: "0x0",
                        dstE: "None",
                        detM: "None"
                    }
                ]
            },
            text: [],
            current_text:[{pc:"*", instr:"------- 目前没有指令 -------"}],
            break_pc: [],
            break_state: false,
            current_pc: "",
            current_id: -1,
            current_cycle: 0,
            terminal: "",
            grid_color: [[],[],[],[],[],[],[],[]],
            progress: 0,
            cpi: 0,
            progress_slide: 0,
            speed: 20,
            ratio: 0,
            cycle_num: 0,
            ins_num: 1,
            timer_flag: false,
            breakpoint_toggle: false
        }
    },
    created: function(){
        
    },
    methods: {
        getFileInfo: function() {
            this.$http.get('/static/json/filename.json').then(function (res){
                this.task_name = JSON.parse(res.bodyText)[0].filename;
            })
        },
        getData: function() {
            this.$http.get('/static/json/data.json').then(function (res){
                this.json_data = JSON.parse(res.bodyText);
                this.cycle_num = this.json_data.CycleNum;
                this.ins_num = this.json_data.InsNum;
                this.ratio = Math.floor(10000/this.cycle_num);
            })
        },
        readFile: function() {
            this.$http.get(this.file_path).then(function(res){
                this.text = [];
                var tmp = res.bodyText.split('\n');
                for(let i = 0; i < tmp.length; ++i)
                {
                    var item = { pc: 0, instr: "" };
                    if(tmp[i][0] == "0" && tmp[i][8] != " " && $.trim(tmp[i].slice(31))[0] != ".")
                    {
                        item.pc = $.trim(tmp[i].slice(0,5));
                        let tmp_ins = $.trim(tmp[i].slice(31));
                        var j;
                        for(j = 0; j < tmp_ins.length; ++j)
                        {
                            if(tmp_ins[j] == "#")
                                break;
                        }
                        item.instr = tmp_ins.slice(0,j);
                        this.text.push(item);
                    }
                }
            });
        },
        checkState: function() {
            if(this.task_name == "None")
            {
                this.$Notice.warning({
                    title: '请先上传文件',
                    duration: 1
                });
                return false;
            }
            return true;
        },
        loadCurrentText: function() {
            this.current_pc = "";
            this.current_id = -1;
            for(let i = 0; i < this.text.length; ++i)
            {
                if(this.info_data[0].PC == this.text[i].pc)
                {
                    this.current_pc = this.text[i].pc;
                    this.current_id = i;
                    break;
                }
            }
            this.current_text = [];
            if(this.current_id != -1)
            {
                for(let j = this.current_id - 2; j <= this.current_id + 2; ++j)
                {
                    if(j >= 0 && j < this.text.length)
                        this.current_text.push(this.text[j]);
                }
            }
            if(this.current_text.length == 0) 
            {
                this.current_text.push({pc:"*", instr:"------- 目前没有指令 -------"})
                this.current_id = -1;
            }
        },
        handleFormatError(file) { 
            this.$Message.error('上传文件 ' + file.name + ' 格式不支持');
        },
        handleUploadSuccess(res, file) {
            this.getFileInfo();
            this.getData();
            this.readFile();
            this.break_pc = [];
            this.progress_slide = 0;
            this.cpi = 0;
            this.$Notice.success({
                title: '上传文件成功!',
                duration: 2
            });
        },
        handleBeforeUpload () {
            this.$refs.upload.clearFiles();
            let fileList = this.$refs.upload.fileList;
            const check = fileList.length < 1;
            if (!check) {
                this.$Notice.warning({
                    title: '最多只能同时上传一个文件',
                    duration: 2
                });
            }
            return check;
        },
        inc_pg() {
            if(this.checkState())
                this.progress_slide += this.ratio;
        },
        dec_pg() {
            if(this.checkState())
                this.progress_slide -= this.ratio;
        },
        reset() {
            if(this.checkState())
            {
                this.progress_slide = 0;
                this.cpi = 0;
            }
        },
        progress_format() {
            return this.progress + "%";
        },
        complete () {
            this.$Notice.success({
                title: '执行完成',
                duration: 2
            });
        },

        start: function(){
            if(!this.timer_flag && this.checkState())
            {
                this.timer = setInterval(this.inc_pg, 1000/this.speed);
                this.timer_flag = true;
            }
        },
        stop: function(){
            if(this.checkState())
            {
                clearInterval(this.timer);
                this.timer_flag = false;
            }
        },
        info_table_class (row, index) {
            return 'info_table';
        },
        reg_table_class (row, index) {
            return 'reg_table';
        },
        handleInfoUpdate () {
            // 懒得整了，就这几个寄存器，就这样写点shitcode吧
            this.current_cycle = Math.floor(this.progress_slide/this.ratio);
            let data = this.json_data.Cycle[this.current_cycle];
            if(data)
            {
                this.info_data[0].PC = data.PC;
                this.info_data[0].State = data.State;
                this.terminal = data.Console.replace(/\n/g,"<br/>");
                for(let i = 0; i < 8; ++i)
                    for(let j = 0; j < 8; ++j)
                        this.grid_color[i][j] = "rgba(" + data.Graphics[j][i].r + "," + data.Graphics[j][i].g + "," + data.Graphics[j][i].b + "," + data.Graphics[j][i].a / 255 + ")";
                for(let key in this.cc_data[0])
                    this.cc_data[0][key] = data.CC[key];
                for(let key in this.reg_data1[0])
                    this.reg_data1[0][key] = data.Register[key];
                for(let key in this.reg_data2[0])
                    this.reg_data2[0][key] = data.Register[key];
                for(let key in this.reg_data3[0])
                    this.reg_data3[0][key] = data.Register[key];
                
                for(let key in this.pipline_reg_data.f_reg_data[0])
                    this.pipline_reg_data.f_reg_data[0][key] = data.PipelineReg.F_Reg[key];
                this.pipline_reg_data.f_reg_data[0]["name"] = "F_REG";
                for(let key in this.pipline_reg_data.d_reg_data[0])
                    this.pipline_reg_data.d_reg_data[0][key] = data.PipelineReg.D_Reg[key];
                this.pipline_reg_data.d_reg_data[0]["name"] = "D_REG";
                for(let key in this.pipline_reg_data.e_reg_data[0])
                    this.pipline_reg_data.e_reg_data[0][key] = data.PipelineReg.E_Reg[key];
                this.pipline_reg_data.e_reg_data[0]["name"] = "E_REG";
                for(let key in this.pipline_reg_data.m_reg_data[0])
                    this.pipline_reg_data.m_reg_data[0][key] = data.PipelineReg.M_Reg[key];
                this.pipline_reg_data.m_reg_data[0]["name"] = "M_REG";
                for(let key in this.pipline_reg_data.w_reg_data[0])
                    this.pipline_reg_data.w_reg_data[0][key] = data.PipelineReg.W_Reg[key];
                this.pipline_reg_data.w_reg_data[0]["name"] = "W_REG";
            }
        },
        BreakPointOk () {
            this.$Message.success('断点设置成功');
        },
        BreakPointCancel () {
            return;
        },
        handleBreakPoint() {
            var len = this.break_pc.length;
            for(let i = 0; i < len; ++i)
            {
                if(this.break_pc[i] == this.current_pc && !this.break_state)
                {
                    this.stop();
                    this.break_state = true;
                }
            }
        }
    },
    computed: {
        pg_color () {
            let color = '#2db7f5';
            if (this.progress == 100) {
                color = '#5cb85c';
            }
            return color;
        },
        cpi_color () {
            let color = '#2db7f5';
            if (this.cpi >= 1.27 && this.cpi < 1.5) {
                color = '#fa8231';
            }
            else if (this.cpi >= 1.5) {
                color = '#ff5500';
            }
            return color;
        },
        CurrentPc() {
            return this.info_data[0].PC;
        }
    },
    watch: {
        progress_slide(val) {
            if(val > 10000 || val < 0)
                return;
            if(val == 10000)
            {
                this.complete();
                this.stop();
            }
            if(val)
                this.cpi = (this.cycle_num/this.ins_num).toFixed(2);
            else
                this.cpi = 0;
            this.handleInfoUpdate();
            this.loadCurrentText();
            this.progress = Math.round(this.progress_slide / 100);
            this.handleBreakPoint()
        },
        current_pc() {
            this.break_state = false;
        }
    },
    template:`
    <Content :style="{padding: '0 50px'}">
            <Modal
                v-model="breakpoint_toggle"
                title="断点设置"
                @on-ok="BreakPointOk"
                @on-cancel="BreakPointCancel">
                <Select v-model="break_pc" style="width:100%;" multiple :max-tag-count="2">
                    <Option v-for="item in text" :value="item.pc">{{ item.instr }}</Option>
                </Select>
            </Modal>
            <div class="topInfo_style">
                <p>基础功能 / </p>
                <p>扩展信息 / </p>
                <p>断点添加 / </p>
                <p>终端输出 / </p>
                <p>彩色矩阵输出 / </p>
            </div>
            
            <Card shadow dis-hover="true">
                <div class="head_cards_warper">
                    <Card class="main_card">
                        <p class="info_title" slot="title"> <Icon size="20" type="ios-home"></Icon> 基本信息 </p>
                        <p class="info_extra" slot="extra"> 当前运行: {{ task_name }}</p>
                        <Table :row-class-name="info_table_class" border :columns="info_list" :data="info_data" ></Table>
                        <br><br>
                        <Table :row-class-name="info_table_class" border :columns="cc_list" :data="cc_data" ></Table>
                    </Card>
                    <Card class="controller_card">
                        <p class="info_title" slot="title"> 
                            <Icon size="20" type="ios-game-controller-a"></Icon> 
                            控制台
                            <p class="progress" slot="extra">进度: {{progress}}%</p> 
                        </p>
                        <div class="controller_wraper">
                            <div class="button_class">
                                <div class="controller_btn">
                                    <Button style="margin-left: 10px" @click="start" icon="md-play" type="primary">开始运行</Button>
                                    <Button style="margin-left: 10px" @click="stop" icon="md-pause" type="primary">停止运行</Button>
                                    <Button style="margin-left: 10px" @click="reset" icon="md-refresh" shape="circle"  :size="20" type="primary"></Button>
                                    <ButtonGroup style="margin-inline: 10px" :size="20" shape="circle">
                                        <Button @click="dec_pg"  :size="20" type="primary">
                                            <Icon type="ios-arrow-back" ></Icon>
                                        </Button>
                                        <Button @click="inc_pg" :size="20" type="primary">
                                            <Icon type="ios-arrow-forward" ></Icon>
                                        </Button>
                                    </ButtonGroup>
                                </div>
                                <Row type="flex" justify="start" style="margin-top: 30px; margin-left: 5px">
                                    <Col span="2" style="height:30px;">
                                        <b style="line-height: 30px; width: 100%">主频 </b>
                                    </Col>
                                    <Col span="14" style="height:30px;">
                                        <Slider v-model="speed" :min="1" :step="1" show-input></Slider>
                                    </Col>
                                    <Col span="2" style="height:30px;">
                                        <b style="line-height: 30px; margin-left: 10px">HZ</b>
                                    </Col>
                                </Row>
                            </div>
                            <div class="controller_circle">
                                <i-circle class="circle" size="130" :percent="progress" :stroke-color="cpi_color" dashboard>
                                    <span class="demo-Circle-inner" style="font-size:20px">{{cpi}} CPI</span>
                                </i-circle>
                                <i-circle class="circle" size="130" :percent="progress" :stroke-color="pg_color">
                                    <Icon v-if="progress == 100" type="ios-checkmark" size="60" style="color:#5cb85c"></Icon>
                                    <span v-else class="demo-Circle-inner" style="font-size:24px">{{progress}} %</span>
                                </i-circle>
                            </div>
                        </div>

                        <Slider v-model="progress_slide" max="10000" :tip-format="progress_format" :step="ratio"></Slider>
                        <Upload
                            ref="upload"
                            type="drag"
                            action="//localhost:7777/post"
                            :format="file_format"
                            :on-format-error="handleFormatError"
                            :on-success="handleUploadSuccess"
                            :before-upload="handleBeforeUpload">
                            <div style="padding: 20px 0">
                                <Icon type="ios-cloud-upload" size="52" style="color: #3399ff"></Icon>
                                <p>点击或拖拽上传文件</p>
                            </div>
                        </Upload>
                    </Card>
                </div>
                <Divider></Divider>
                <div class="body_card_wraper">
                    <Card class="reg_card">
                        <p slot="title"><Icon size="20" type="ios-cog"></Icon> 寄存器</p>
                        <div style="min-height: 200px;">
                            <Table :row-class-name="reg_table_class" border stripe :columns="reg_list1" :data="reg_data1" ></Table>
                            <Table :row-class-name="reg_table_class" border stripe :columns="reg_list2" :data="reg_data2" ></Table>
                            <Table :row-class-name="reg_table_class" border stripe :columns="reg_list3" :data="reg_data3" ></Table>
                        </div>
                    </Card>
                    <Card class="now_card" v-on:click.native="breakpoint_toggle=true">
                        <p slot="title"><Icon size="20" type="md-code"></Icon> 当前指令</p>
                        <p class="info_extra" slot="extra">当前Cycle: {{current_cycle + 1}}</p>
                        <div style="min-height: 200px;">
                            <List border item-layout="vertical">
                                <ListItem v-for="(item, i) in current_text">
                                    <p v-if="item.pc == CurrentPc" style="display:flex">
                                        <Tag type="border" style="margin-right:10px">{{item.pc}}</Tag> 
                                        <Tag color="primary" size="large"><b>{{item.instr}}</b></Tag>
                                    </p>
                                    <p v-else style="display:flex">
                                        <Tag type="border" style="margin-right:10px">{{item.pc}}</Tag> 
                                        <Tag color="default" size="large"><b>{{item.instr}}</b></Tag>
                                    </p>
                                </ListItem>
                            </List>
                        </div>
                    </Card>
                    <Card class="terminal_card">
                        <p slot="title"><Icon size="20" type="md-grid"></Icon> 终端</p>
                        <p v-if="terminal == ''">None Output</p>
                        <b class="terminal_text" v-else v-html="terminal"></b>
                        <div class="terminal_grid">
                            <span v-for="list in grid_color">
                                <div v-for="item in list" class="grid_block" :style="{'background-color':item}"></div>
                            </span>
                        </div>
                    </Card>
                </div>
                <Divider></Divider>
                <div class="footer_card_wraper">
                    <Card class="pipeline_card">
                        <p slot="title"><Icon size="20" type="md-aperture"></Icon> 流水线寄存器</p>
                        <div style="min-height: 200px;">
                            <Table no-data-text="None" :row-class-name="reg_table_class" border :columns="pipline_reg_list.f_reg_list" :data="pipline_reg_data.f_reg_data" ></Table>
                            <br><br>
                            <Table no-data-text="None" :row-class-name="reg_table_class" border :columns="pipline_reg_list.d_reg_list" :data="pipline_reg_data.d_reg_data" ></Table>
                            <br><br>
                            <Table no-data-text="None" :row-class-name="reg_table_class" border :columns="pipline_reg_list.e_reg_list" :data="pipline_reg_data.e_reg_data" ></Table>
                            <br><br>
                            <Table no-data-text="None" :row-class-name="reg_table_class" border :columns="pipline_reg_list.m_reg_list" :data="pipline_reg_data.m_reg_data" ></Table>
                            <br><br>
                            <Table no-data-text="None" :row-class-name="reg_table_class" border :columns="pipline_reg_list.w_reg_list" :data="pipline_reg_data.w_reg_data" ></Table>
                        </div>
                    </Card>
                </div>
            </Card>
    </Content>
    `
}
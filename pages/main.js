export default {
    data: function (){
        return {
            task_name:"None",
            file_path: "",
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
                        predPC: "0x001"
                    }
                ],
                d_reg_data: [
                    {
                        name: "D_REG",
                        Stat: "Bubble",
                        icode: "3",
                        ifun : "0",
                        rA: "None",
                        rB: "RCX",
                        valC: "0x64",
                        valP: "0x14",
                    }
                ],
                e_reg_data: [
                    {
                        name: "E_REG",
                        Stat: "Stall",
                        icode: "3",
                        ifun: "0",
                        valC: "0x0",
                        valA: "0x1bd5b7dde",
                        valB: "0x1bd5b7dde",
                        srcA: "None",
                        srcB: "None",
                        dstE: "RAX",
                        detM: "None"
                    }
                ],
                m_reg_data: [
                    {
                        name: "M_REG",
                        Stat: "AOK",
                        icode: "0",
                        valE: "0x1bd5b7dde",
                        valA: "0x1bd5b7dde",
                        dstE: "None",
                        detM: "None"
                    }
                ],
                w_reg_data: [
                    {
                        name: "W_REG",
                        Stat: "AOK",
                        icode: "hhhh",
                        valE: "123123123",
                        valM: "0x0",
                        dstE: "None",
                        detM: "None"
                    }
                ]
            },
            text: [],
            current_text:[{pc:"*", instr:"------- 目前没有指令 -------"}],
            current_pc: "",
            current_id: -1,
            terminal: "",

            progress: 0,
            cpi: 0,
            progress_slide: 0,
            speed: 20,
            ratio: 0,
            cycle_num: 0,
            timer_flag: false
        }
    },
    created: function(){
        this.getData();
    },
    methods: {
        getData: function() {
            this.$http.jsonp('/data.json', {jsonpCallback: "json_data"}).then(function (res){
                this.json_data = JSON.parse(res.bodyText);
                //this.task_name = this.json_data.fileName;
                this.file_path = "../static/source/tmp.yo";
                this.cycle_num = this.json_data.CycleNum;
                this.ratio = Math.floor(10000/this.cycle_num);
            })
        },
        readFile: function() {
            this.$http.get(this.file_path).then(function(res){
                var tmp = res.bodyText.split('\n');
                for(let i = 0; i < tmp.length; ++i)
                {
                    var item = { pc: 0, instr: "" }; 
                    if(tmp[i][0] == "0")
                    {
                        item.pc = tmp[i].slice(0,5);
                        let tmp_ins = $.trim(tmp[i].slice(29));
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
            this.getData();
        },
        handleBeforeUpload () {
            this.$refs.upload.clearFiles();
            let fileList = this.$refs.upload.fileList;
            const check = fileList.length < 1;
            if (!check) {
                this.$Notice.warning({
                    title: '最多只能同时上传一个文件'
                });
            }
            return check;
        },
        inc_pg() {
            this.progress_slide += this.ratio;
        },
        dec_pg() {
            this.progress_slide -= this.ratio;
        },
        reset() {
            this.cpi = 0;
            this.progress_slide = 0;
        },
        progress_format() {
            return this.progress + "%";
        },
        complete () {
            this.$Notice.success({
                title: '执行完成',
                duration: 1
            });
        },
        start: function(){
            if(!this.timer_flag)
            {
                this.timer = setInterval(this.inc_pg, 1000/this.speed);
                this.timer_flag = true;
            }
        },
        stop: function(){
            clearInterval(this.timer);
            this.timer_flag = false;
        },
        info_table_class (row, index) {
            return 'info_table';
        },
        reg_table_class (row, index) {
            return 'reg_table';
        },
        handleInfoUpdate () {
            // 懒得整了，就这几个寄存器，就这样写点shitcode吧
            let cycle_key = Math.floor(this.progress_slide/this.ratio);
            let data = this.json_data.Cycle[cycle_key];
            if(data)
            {
                this.info_data[0].PC = data.PC;
                this.info_data[0].State = data.State;
                this.terminal = data.Console;
                console.log(this.terminal);
                
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
                this.stop();
            this.progress = this.progress_slide / 100;
            this.cpi = (this.text.length/this.cycle_num).toFixed(2);
            this.handleInfoUpdate();
        },
        file_path() {
            this.readFile();
        },
        info_data: {
            handler() {
                this.loadCurrentText();
            },
            deep: true
        }
    },
    template:`
    <Content :style="{padding: '0 50px'}">
            <div class="topInfo_style">
                <p>基础功能 / </p>
                <p>寄存器信息 / </p>
                <p>内存信息 / </p>
                <p>PC / </p>
                <p>CC / </p>
                <p>CPI</p>
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
                                <Row type="flex" justify="start" :gutter="2">
                                    <Col span="4" style="height: 30px">
                                        <Button @click="start" type="primary"><Icon suze="30" type="md-arrow-round-forward" color="#ecf0f1"></Icon> 开始运行 </Button>
                                    </Col>
                                    <Col span="4" style="height: 30px">
                                        <Button @click="stop" type="primary"><Icon suze="20" type="md-pause" color="#ecf0f1"></Icon> 停止运行</Button>
                                    </Col>
                                    <Col span="6" style="margin-left: 5%">
                                        <button-group :size="30" shape="circle">
                                            <Button @click="dec_pg"  :size="20" type="primary">
                                                <Icon type="ios-arrow-back" ></Icon>
                                                后退
                                            </Button>
                                            <Button @click="inc_pg" :size="20" type="primary">
                                                前进
                                                <Icon type="ios-arrow-forward" ></Icon>
                                            </Button>
                                        </button-group>
                                    </Col>
                                    <Col span="3" style="margin-left: 2%">
                                        <Button @click="reset" shape="circle"  :size="20" type="primary">
                                            <Icon type="md-refresh" ></Icon>
                                        </Button>
                                    </Col>
                                </Row>
                                <Row type="flex" justify="start" style="margin-top: 30px; margin-left: 5px">
                                    <Col span="2" style="height:30px;">
                                        <b style="line-height: 30px; width: 100%">主频</b>
                                    </Col>
                                    <Col span="14" style="height:30px;">
                                        <Slider v-model="speed" :min="1" :step="1" show-input></Slider>
                                    </Col>
                                    <Col span="2" style="height:30px;">
                                        <b style="line-height: 30px; margin-left: 10px">HZ</b>
                                    <Col>
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
                        <p slot="title"><Icon size="20" type="ios-cog"></Icon> 寄存器 Registers</p>
                        <div style="min-height: 200px;">
                            <Table :row-class-name="reg_table_class" border stripe :columns="reg_list1" :data="reg_data1" ></Table>
                            <Table :row-class-name="reg_table_class" border stripe :columns="reg_list2" :data="reg_data2" ></Table>
                            <Table :row-class-name="reg_table_class" border stripe :columns="reg_list3" :data="reg_data3" ></Table>
                        </div>
                    </Card>
                    <Card class="now_card">
                        <p slot="title"><Icon size="20" type="ios-cog"></Icon> 当前指令 Current Ins</p>
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
                        <p slot="title"><Icon size="20" type="ios-cog"></Icon> 终端 Terminal</p>
                        <p v-if="terminal == ''">None Output</p>
                        <b class="terminal_text" v-else>{{terminal}}</b>
                    </Card>
                </div>
                <Divider></Divider>
                <div class="footer_card_wraper">
                    <Card class="pipeline_card">
                        <p slot="title"><Icon size="20" type="md-aperture"></Icon> 流水线寄存器</p>
                        <div style="min-height: 200px;">
                            <Table :row-class-name="reg_table_class" border :columns="pipline_reg_list.f_reg_list" :data="pipline_reg_data.f_reg_data" ></Table>
                            <br><br>
                            <Table :row-class-name="reg_table_class" border :columns="pipline_reg_list.d_reg_list" :data="pipline_reg_data.d_reg_data" ></Table>
                            <br><br>
                            <Table :row-class-name="reg_table_class" border :columns="pipline_reg_list.e_reg_list" :data="pipline_reg_data.e_reg_data" ></Table>
                            <br><br>
                            <Table :row-class-name="reg_table_class" border :columns="pipline_reg_list.m_reg_list" :data="pipline_reg_data.m_reg_data" ></Table>
                            <br><br>
                            <Table :row-class-name="reg_table_class" border :columns="pipline_reg_list.w_reg_list" :data="pipline_reg_data.w_reg_data" ></Table>
                        </div>
                    </Card>
                </div>
            </Card>
    </Content>
    `
}
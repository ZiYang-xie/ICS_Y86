let app = new Vue({
    el: '#app',
    data: {
        json_data:"",
        reg_list1: [
            {
                title: "Rax",
                key: "rax",
                align: 'center'
            },
            {
                title: "Rcx",
                key: "rcx",
                align: 'center'
            },
            {
                title: "Rdx",
                key: "rdx",
                align: 'center'
            },
            {
                title: "Rbx",
                key: "rbx",
                align: 'center'
            },
            {
                title: "Rsp",
                key: "rsp",
                align: 'center'
            }
        ],
        reg_list2: [
            {
                title: "Rbp",
                key: "rbp",
                align: 'center'
            },
            {
                title: "Rsi",
                key: "rsi",
                align: 'center'
            },
            {
                title: "Rdi",
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
                rax: "0x1",
                rcx: "0x2",
                rdx: "0xffffffffffffffff",
                rbx: "0x23",
                rsp: "0x1"
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
                PC: 2
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
                cc: "Condition Codes",
                zf: 1,
                sf: 0,
                of: 0
            }
        ],
        progress: 0,
        speed: 80,
        ratio: 0,
        cycleNum: 0,
        timerFlag: false
    },
    created: function(){
        this.$http.jsonp('static/json/data.json', {jsonpCallback: "json_data"}).then(function (res){
            this.json_data = JSON.parse(res.bodyText);
            this.cycleNum = this.json_data.CycleNum;
            this.ratio = Math.floor(100/this.cycleNum);
        })
    },
    methods: {
        getData: function(){
            console.log(this.json_data);
        },
        inc_pg: function() {
            this.progress += this.ratio;
        },
        dec_pg: function() {
            this.progress -= this.ratio;
        },
        progress_format: function() {
            return this.progress + "%";
        },
        complete () {
            this.$Notice.success({
                title: '执行完成',
                duration: 1
            });
        },
        start: function(){
            if(!this.timerFlag)
            {
                if(this.ratio > 20)
                    this.timer = setInterval(this.inc_pg, 700 - 3*this.speed);
                else if(this.ratio > 10)
                    this.timer = setInterval(this.inc_pg, 600 - 3*this.speed);
                else if(this.ratio > 10)
                    this.timer = setInterval(this.inc_pg, 500 - 3*this.speed);
                this.timerFlag = true;
            }
        },
        stop: function(){
            clearInterval(this.timer);
            this.timerFlag = false;
        },
        info_table_class (row, index) {
            return 'info_table';
        },
        reg_table_class (row, index) {
            return 'reg_table';
        },
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
            if (this.progress >= 50 && this.progress < 70) {
                color = '#fa8231';
            }
            else if (this.progress >= 70) {
                color = '#ff5500';
            }
            return color;
        }
    },
    watch: {
        progress(val) {
            if(val >= 100)
            {
                this.stop();
            }
            let data = this.json_data.Cycle[Math.floor(this.progress/this.ratio)];

            this.info_data[0].PC = data.PC;
            this.info_data[0].State = data.State;
            for(let key in this.cc_data[0])
                this.cc_data[0][key] = data.CC[key];
            for(let key in this.reg_data1[0])
                this.reg_data1[0][key] = data.Register[key];
            for(let key in this.reg_data2[0])
                this.reg_data2[0][key] = data.Register[key];
            for(let key in this.reg_data3[0])
                this.reg_data3[0][key] = data.Register[key];
        }
    }
})
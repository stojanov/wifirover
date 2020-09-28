window.onload = main;

const moveBtnIDName = [
    [ "moveright", 1 ],
    [ "moveleft", 2 ],
    [ "movefwd", 3 ],
    [ "movebwd", 4 ],
]

let ip = null;
const getIP = input => ip = "http://" + input.value + ":80/"; // pointer

const moveRover = (ip, direction) => {
    $.get(ip(), {move: direction})
};

$.ajaxSetup({timeout:1000});

function main() {
    const ipinp = document.getElementById('ipaddr');
    const fnIP = getIP.bind(this, ipinp);

    moveBtnIDName
        .map(btn => [
            document.getElementById(btn[0]), 
            btn[1]
        ])
        .forEach(btn => {
            btn[0].addEventListener('click', moveRover.bind(this, fnIP, btn[1] ));
        });
}
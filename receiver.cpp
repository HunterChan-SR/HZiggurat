// #include <iostream>
// #include <vector>
// #include <cryptoTools/Common/Defines.h>
// #include <cryptoTools/Common/BitVector.h>
// #include <cryptoTools/Crypto/PRNG.h>
// #include <cryptoTools/Network/IOService.h>
// #include <cryptoTools/Network/Endpoint.h>
// #include <libOTe/TwoChooseOne/Iknp/IknpOtExtReceiver.h>
// #include <macoro/sync_wait.h> // 用于同步等待异步任务

// using namespace osuCrypto;

// int main(int argc, char** argv)
// {
//     // 初始化网络服务和客户端端点（连接到服务端）
//     IOService ios;
//     Endpoint ep(ios, "localhost", 1212, EpMode::Client);
//     auto channel = ep.addChannel();

//     // 初始化 OT 接收者
//     IknpOtExtReceiver receiver;
    
//     const u64 numOTs = 128;
//     // 使用 osuCrypto::BitVector 存放选择比特
//     BitVector choices(numOTs);
//     std::vector<block> recvMessages(numOTs);
    
//     // 初始化选择比特：偶数索引选择 true，奇数索引选择 false
//     for (u64 i = 0; i < numOTs; ++i) {
//         choices[i] = (i % 2 == 0);
//     }
    
//     // 创建 PRNG 对象
//     PRNG prng(ZeroBlock);
//     // 从 Channel 获取 Socket 引用
//     auto& sock = channel.get();

//     // 异步调用 OT 接收，并同步等待任务完成
//     macoro::sync_wait(receiver.receive(choices, recvMessages, prng, sock));

//     channel.close();
//     ep.stop();
//     ios.stop();
    
//     // 打印接收到的消息（block 的输出取决于库的 operator<< 定义）
//     for (u64 i = 0; i < numOTs; ++i)
//     {
//         std::cout << "OT " << i << ", choice=" << (int)choices[i]
//                   << " -> " << recvMessages[i] << std::endl;
//     }

//     std::cout << "Receiver OT transfer complete." << std::endl;
//     return 0;
// }
int main(){return 0;}
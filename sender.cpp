// #include <iostream>
// #include <vector>
// #include <array>
// #include <cryptoTools/Common/Defines.h>
// #include <cryptoTools/Crypto/PRNG.h>
// #include <cryptoTools/Network/IOService.h>
// #include <cryptoTools/Network/Endpoint.h>
// #include <libOTe/TwoChooseOne/Iknp/IknpOtExtSender.h>
// #include <macoro/sync_wait.h> // 用于同步等待异步任务

// using namespace osuCrypto;

// int main(int argc, char** argv)
// {
//     // 初始化网络服务和监听端点（服务端）
//     IOService ios;
//     Endpoint ep(ios, "localhost", 1212, EpMode::Server);
//     auto channel = ep.addChannel();

//     // 初始化 OT 发送者
//     IknpOtExtSender sender;
    
//     const u64 numOTs = 128;
//     // 构造每个 OT 的候选消息（2 个 block 组成一对）
//     std::vector<std::array<block, 2>> sendMessages(numOTs);
//     for (u64 i = 0; i < numOTs; ++i) {
//         sendMessages[i][0] = toBlock(i, 0);
//         sendMessages[i][1] = toBlock(i, 1);
//     }
    
//     // 创建 PRNG 对象，用于 OT 扩展
//     PRNG prng(ZeroBlock);
//     // 从 Channel 获取底层的 Socket（注意：不同版本可能名称不同，这里使用 channel.get()）
//     auto& sock = channel.get();

//     // 异步调用 OT 发送，并同步等待任务完成
//     macoro::sync_wait(sender.send(sendMessages, prng, sock));

//     channel.close();
//     ep.stop();
//     ios.stop();

//     std::cout << "Sender OT transfer complete." << std::endl;
//     return 0;
// }
int main(){return 0;}
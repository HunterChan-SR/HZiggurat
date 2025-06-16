#include <libOTe/NChooseOne/Kkrt/KkrtDefines.h>
#include <libOTe/NChooseOne/Kkrt/KkrtNcoOtReceiver.h>
#include <libOTe/NChooseOne/Kkrt/KkrtNcoOtSender.h>

#include <libOTe/NChooseOne/Oos/OosDefines.h>
#include <libOTe/NChooseOne/Oos/OosNcoOtReceiver.h>
#include <libOTe/NChooseOne/Oos/OosNcoOtSender.h>

#include <libOTe/NChooseOne/SoftSpokenOT/NOneSemiHonest.h>

#include <cryptoTools/Network/Session.h>
#include <cryptoTools/Network/IOService.h>
#include <cryptoTools/Common/Log.h>
#include <cryptoTools/Crypto/PRNG.h>
#include <cryptoTools/Common/TestCollection.h>

#include <chrono>
#include <iostream>

using namespace osuCrypto;


double elapse_ms(std::chrono::_V2::system_clock::time_point begin,std::chrono::_V2::system_clock::time_point end){
    auto elapse_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    return elapse_ns.count() * 1e-6;
}
double elapse_s(std::chrono::_V2::system_clock::time_point begin,std::chrono::_V2::system_clock::time_point end){
    auto elapse_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    return elapse_ns.count() * 1e-9;
}

inline auto eval(macoro::task<> &t0, macoro::task<> &t1)
{
    auto r = macoro::sync_wait(macoro::when_all_ready(std::move(t0), std::move(t1)));
    std::get<0>(r).result();
    std::get<1>(r).result();
}

void setBaseOts(NcoOtExtSender &sender,
                NcoOtExtReceiver &recv)
{

    auto begin = std::chrono::high_resolution_clock::now();

    u64 baseCount = sender.getBaseOTCount();

    std::vector<block> baseRecv(baseCount);
    std::vector<std::array<block, 2>> baseSend(baseCount);
    BitVector baseChoice(baseCount);
    PRNG prng0(ZeroBlock);
    PRNG prng1(OneBlock);
    baseChoice.randomize(prng0);

    prng0.get((u8 *)baseSend.data()->data(), sizeof(block) * 2 * baseSend.size());
    for (u64 i = 0; i < baseCount; ++i)
    {
        baseRecv[i] = baseSend[i][baseChoice[i]];
    }

    auto sock = cp::LocalAsyncSocket::makePair();

    auto p0 = sender.setBaseOts(baseRecv, baseChoice, sock[0]);
    auto p1 = recv.setBaseOts(baseSend, prng0, sock[1]);

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "BaseOts setup time: " << elapse_ms(begin,end) << " ms" << std::endl;

    auto begin2 = std::chrono::high_resolution_clock::now();
    eval(p0, p1);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::cout << "BaseOts eval time: " << elapse_ms(begin2,end2) << " ms" << std::endl;
    
    for(int i=0;i<baseCount;++i)
    {
        std::cout<<"i: "<< i << " baseChoice: " <<baseChoice[i] << " baseRecv: " << baseRecv[i] << " baseSend[0]: " << baseSend[i][0] << " baseSend[1]: " << baseSend[i][1] << std::endl;
    }
    std::cout<<"baseOt passed"<<std::endl;
}
// void testNco(
//     NcoOtExtSender &sender,
//     const u64 &numOTs,
//     PRNG &prng0,
//     NcoOtExtReceiver &recv,
//     PRNG &prng1)
// {
//     auto sock = cp::LocalAsyncSocket::makePair();

//     u64 stepSize = 33;
//     std::vector<block> inputs(stepSize);
//     setThreadName("Receiver");

//     for (size_t j = 0; j < 3; j++)
//     {
//         // perform the init on each of the classes. should be performed concurrently

//         auto p0 = sender.init(numOTs, prng0, sock[0]);
//         auto p1 = recv.init(numOTs, prng1, sock[1]);
//         eval(p0, p1);

//         std::vector<block> encoding1(stepSize), encoding2(stepSize);

//         // Get the random OT mMessages
//         for (u64 i = 0; i < numOTs; i += stepSize)
//         {
//             auto curStepSize = std::min<u64>(stepSize, numOTs - i);
//             std::vector<u8> skips(curStepSize);

//             prng0.get(inputs.data(), inputs.size());
//             prng0.get((bool *)skips.data(), skips.size());

//             for (u64 k = 0; k < curStepSize; ++k)
//             {

//                 // The receiver MUST encode before the sender. Here we are only calling encode(...)
//                 // for a single i. But the receiver can also encode many i, but should only make one
//                 // call to encode for any given value of i.
//                 if (skips[k])
//                 {
//                     recv.zeroEncode(i + k);
//                 }
//                 else
//                 {
//                     recv.encode(i + k, &inputs[k], (u8 *)&encoding1[k], sizeof(block));
//                 }
//             }

//             // This call will send to the other party the next "curStepSize " corrections to the sender.
//             // If we had made more or less calls to encode above (for contigious i), then we should replace
//             // curStepSize  with however many calls we made. In an extreme case, the reciever can perform
//             // encode for i \in {0, ..., numOTs - 1}  and then call sendCorrection(recvChl, numOTs).
//             auto p1 = recv.sendCorrection(sock[1], curStepSize);

//             // receive the next curStepSize  correction values. This allows the sender to now call encode
//             // on the next curStepSize  OTs.
//             auto p0 = sender.recvCorrection(sock[0], curStepSize);

//             eval(p0, p1);

//             for (u64 k = 0; k < curStepSize; ++k)
//             {
//                 // the sender can now call encode(i, ...) for k \in {0, ..., i}.
//                 // Lets encode the same input and then we should expect to
//                 // get the same encoding.
//                 sender.encode(i + k, &inputs[k], (u8 *)&encoding2[k], sizeof(block));

//                 // check that we do in fact get the same value
//                 if (!skips[k] && neq(encoding1[k], encoding2[k]))
//                     throw UnitTestFail("ot[" + std::to_string(i + k) + "] not equal " LOCATION);

//                 // In addition to the sender being able to obtain the same value as the receiver,
//                 // the sender can encode and other codeword. This should result in a different
//                 // encoding.
//                 inputs[k] = prng0.get<block>();

//                 sender.encode(i + k, &inputs[k], (u8 *)&encoding2[k], sizeof(block));

//                 if (eq(encoding1[k], encoding2[k]))
//                     throw UnitTestFail(LOCATION);
//             }
//         }

//         p0 = sender.check(sock[0], prng0.get());
//         p1 = recv.check(sock[1], prng1.get());

//         eval(p0, p1);
//     }
// }

// void Kkrt()
// {
//     setThreadName("Sender");
//     PRNG prng0(block(4253465, 3434565));
//     PRNG prng1(block(42532335, 334565));

//     u64 numOTs = 80;
//     u64 inputSize = 8;

//     KkrtNcoOtSender sender;
//     KkrtNcoOtReceiver recv;
//     // u64  baseCount;
//     sender.configure(true, 40, inputSize);
//     recv.configure(true, 40, inputSize);

//     setBaseOts(sender, recv);

//     auto messageCount = 1ull << inputSize;
//     Matrix<block> sendMessage(numOTs, messageCount);
//     std::vector<block> recvMessage(numOTs);

//     prng0.get(sendMessage.data(), sendMessage.size());

//     std::vector<u64> choices(numOTs);
//     for (u64 i = 0; i < choices.size(); ++i)
//     {
//         choices[i] = prng0.get<u8>();
//     }

//     auto sock = cp::LocalAsyncSocket::makePair();

//     auto p0 = recv.receiveChosen(messageCount, recvMessage, choices, prng1, sock[0]);
//     auto p1 = sender.sendChosen(sendMessage, prng0, sock[1]);

//     eval(p0, p1);

//     for (u64 i = 0; i < choices.size(); ++i)
//     {
//         if (neq(recvMessage[i], sendMessage(i, choices[i])))
//             throw UnitTestFail("bad message " LOCATION);
//     }
// }

void Oos(u64 numOTs = 80, u64 inputSize = 8, u64 statSecParam = 128, bool maliciousSecure = false)
{

  
   
    PRNG prng0(block(4253465, 3434565));
    PRNG prng1(block(42532335, 334565));

    // u64 numOTs = 80;
    // u64 inputSize = 8;

    OosNcoOtSender sender;
    OosNcoOtReceiver recv;
    // u64  baseCount;
    sender.configure(maliciousSecure, statSecParam, inputSize);
    recv.configure(maliciousSecure, statSecParam, inputSize);

    setBaseOts(sender, recv);

    auto begin = std::chrono::high_resolution_clock::now();
    auto messageCount = 1ull << inputSize;
    Matrix<block> sendMessage(numOTs, messageCount);
    std::vector<block> recvMessage(numOTs);

    prng0.get(sendMessage.data(), sendMessage.size());

    std::vector<u64> choices(numOTs);
    for (u64 i = 0; i < choices.size(); ++i)
    {
        choices[i] = prng0.get<u8>();
    }

    auto sock = cp::LocalAsyncSocket::makePair();

    auto p0 = recv.receiveChosen(messageCount, recvMessage, choices, prng1, sock[0]);
    auto p1 = sender.sendChosen(sendMessage, prng0, sock[1]);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "numOTs: " << numOTs << " messageCount: " << (1ull << inputSize)  << " statSecParam: " << statSecParam << " maliciousSecure: " << maliciousSecure << std::endl;
    std::cout << "OosNcoOt setup time: " << elapse_ms(begin,end) << " ms" << std::endl;

    auto begin2 = std::chrono::high_resolution_clock::now();
    eval(p0, p1);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::cout << "OosNcoOt eval time: " << elapse_ms(begin2,end2) << " ms" << std::endl;
    std::cout << "1-out-of-"<<messageCount <<" OT: "<<  numOTs/ elapse_s(begin2,end2) <<" OTps"<<std::endl;
    
    for (u64 i = 0; i < choices.size(); ++i)
    {
        std::cout << "i: " << i << " choice: " << choices[i] << " sendMessage[" << choices[i] << "]: " << sendMessage(i, choices[i]) << " recvMessage: " << recvMessage[i] << std::endl;
        if (neq(recvMessage[i], sendMessage(i, choices[i])))
            throw UnitTestFail("bad message " LOCATION);
    }
    std::cout << "NcoOt_chosen passed" << std::endl;
}

int main()
{

    u64 numOTs = 1000, inputSize = 9;
    // auto begin = std::chrono::high_resolution_clock::now();
    Oos(numOTs, inputSize);
    // auto end = std::chrono::high_resolution_clock::now();
    // auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    // std::cout << "numOTs:" << numOTs << " inputSize:" << inputSize << " NcoOt_chosen " << elapsed.count() << " nanoseconds." << std::endl;

    return 0;
}
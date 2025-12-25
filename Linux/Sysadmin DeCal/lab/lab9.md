> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [decal.ocf.berkeley.edu](https://decal.ocf.berkeley.edu/archives/2021-fall/labs/a9/)

Lab 9 - Security  Lab 9 - 安全
============================

Facilitator: [Mark Zhang](/staff#mark-zhang)  引导员：Mark Zhang

34 min read

[](#table-of-contents)Table of contents   目录
--------------------------------------------

1.  [Overview  概述](#overview)
    1.  [Getting help  获取帮助](#getting-help)
2.  [Threat Models  威胁模型](#threat-models)
    1.  [Example: Safekeeping  示例：安全保管](#example-safekeeping)
    2.  [Task 1: Construct your own threat model  
        任务 1: 构建自己的威胁模型](#task-1-construct-your-own-threat-model)
3.  [Security Building Blocks  
    安全构建模块](#security-building-blocks)
    1.  [Encryption  加密](#encryption)
        1.  [A Brief Introduction  简要介绍](#a-brief-introduction)
        2.  [Types of Encryption  
            加密类型](#types-of-encryption)
            1.  [Symmetric Key Cryptography  
                对称密钥加密](#symmetric-key-cryptography)
            2.  [Public Key Cryptography  公钥密码学](#public-key-cryptography)
    2.  [Signatures and Certificates  
        签名和证书](#signatures-and-certificates)
    3.  [Hashing  哈希](#hashing)
4.  [Encryption Lab Activity](#encryption-lab-activity)
    1.  [Submission  提交](#submission)
    2.  [Introduction: Making SSH keys  
        介绍：生成 SSH 密钥](#introduction-making-ssh-keys)
    3.  [Warm-up Task  预热任务](#warm-up-task)
    4.  [Task 2 - Symmetric Encryption  
        任务 2 - 对称加密](#task-2---symmetric-encryption)
    5.  [Task 3 - Asymmetric Encryption  
        任务 3 - 非对称加密](#task-3---asymmetric-encryption)
    6.  [Task 4 - Hashing  
        任务 4 - 哈希](#task-4---hashing)
    7.  [File Security   文件安全](#file-security)
        1.  [File Security and Permissions  
            文件安全与权限](#file-security-and-permissions)
        2.  [Task 5 - File Security  
            任务 5 - 文件安全](#task-5---file-security)
    8.  [Network Security  网络安全](#network-security)
        1.  [Task 6 - Network Security Lab Activity  
            任务 6 - 网络安全实验室活动](#task-6---network-security-lab-activity)
    9.  [Optional Task: Letsencrypt on an nginx instance!  
        可选任务：在 nginx 实例上使用 letsencrypt！](#optional-task-letsencrypt-on-an-nginx-instance)
5.  [Footnotes  脚注](#footnotes)

* * *

[](#overview)Overview   概述
==========================

In this lab, we will cover a variety of topics that are of interest to those studying computer security. We will go much more in depth than you need to know, so we hope you pick up the main practical concepts and dig deeper if you are interested.  
在本次实验中，我们将介绍一些对计算机安全研究者感兴趣的话题。我们将深入探讨比你所需了解的更多内容，因此希望你能掌握主要的实用概念，并在感兴趣的情况下进一步深入研究。

There are many aspects to security, and the field spans a number of disciplines. We will cover the following:  
安全涉及许多方面，该领域涵盖多个学科。我们将涵盖以下内容：

*   Threat Models  威胁模型
*   Security Building Blocks  
    安全构建模块
    *   Encryption - Symmetric and Public-Key Crypto  
        加密 - 对称加密和公钥密码
    *   Certs, Signatures  证书，签名
    *   Hashing  哈希
*   File Security  文件安全
*   Network Security  网络安全

[](#getting-help)Getting help   获取帮助
------------------------------------

If you want any help with any part of this lab, join the OCF slack/discord ([https://ocf.io/slack](https://ocf.io/slack)), and post your questions in **#decal-general**.  
如果你在本实验的任何部分需要帮助，请加入 OCF 的 slack/discord（ https://ocf.io/slack），并在#decal-general 频道提问。

[](#threat-models)Threat Models   威胁模型
======================================

The most important thing to remember when designing secure systems is understanding your threat model. No system is guaranteed to be secure or able to withstand all attacks, nor is this even possible in the face of extreme adversaries. However, you can (and should) take precautions against the threats you are likely to face. Balancing the need for authorized users to get access to the system while keeping unauthorized users out is very easy to get wrong. Fortunately, smart people have distilled the principles of security down to a few axioms, covered very well in the first [lecture note](http://www.icir.org/vern/cs161-sp17/notes/Principles.1.19.pdf) of CS 161 (credit to Prof. David Wagner). It is recommended to read the lecture note.  
在设计安全系统时，最重要的是理解你的威胁模型。没有任何系统能保证绝对安全或能抵御所有攻击，尤其是在面对极端对手时，这甚至是不可能的。然而，你可以（也应该）采取措施来防范你很可能遇到的威胁。在允许授权用户访问系统的同时阻止非授权用户进入，是非常容易出错的。幸运的是，聪明的人已经将安全原则提炼为几条公理，这些内容在 CS 161 的第一讲笔记中讲得非常清楚（感谢 Prof. David Wagner）。建议阅读这些讲义。

**When constructing a threat model, keep questions such as these in mind:  
在构建威胁模型时，请记住这些问题：**

1.  What are you protecting?  
    你在保护什么？
2.  Who are your adversaries?  
    你的对手是谁？
3.  How likely is it you will need to protect it?  
    你有多大可能需要保护它？
4.  What are the consequences of failing to protect it?  
    未能保护它会带来什么后果？
5.  How many resources should you devote to protecting it?  
    你应该投入多少资源来保护它？

[](#example-safekeeping)Example: Safekeeping   示例：安全保管
------------------------------------------------------

Let’s say that you run a safe storage facility for customers to store a variety of valuables. **Here’s a description of what your threat model might look like:**  
假设你经营一个安全存储设施，供客户存放各种贵重物品。以下是你威胁模型的描述：

In this scenario, you’re responsible for your clients’ valuables. There are a multitude of adversaries, including burglars ranging from smash-and-grab to more sophisticated attackers, disgruntled employees, clients looking to claim fradulent loss, and natural disasters such as earthquakes, fires, or tornados.  
在这个情境中，你负责客户的贵重物品。存在多种对手，包括从快速抢夺到更高级攻击的盗贼，不满的员工，试图声称欺诈损失的客户，以及自然灾害如地震、火灾或龙卷风。

Storage facilties containing potentially high amounts of valuables in close proximity to each other may serve as enticing targets for attackers and failing to protect the facility would result in a loss of trust that would be devastating for a business.  
存储设施中存放大量贵重物品且彼此靠近，可能会成为攻击者的诱人目标，未能保护该设施将导致信任的丧失，这对企业来说将是毁灭性的。

In order to protect against burglars, an variety of protections can be enabled from increased surveillance to 24/7 guards. The rational amount of security depends on the level of desired profit and likelihood of a break-in.  
为了防止入室盗窃，可以启用多种保护措施，从增加监控到全天候保安。合理的安全措施取决于期望的利润水平和发生入室盗窃的可能性。

Allowing clients to install their own security protects allows them protection against both disgruntled employees and acts as a protection mechanism against fradulent clients.  
允许客户安装自己的安全措施，可以保护他们免受不满员工的侵害，同时也作为一种防止欺诈客户的保护机制。

[](#task-1-construct-your-own-threat-model)Task 1: Construct your own threat model   任务 1：构建你自己的威胁模型
----------------------------------------------------------------------------------------------------

You’re a journalist criticizing the local authoritarian government and trying to get your story to ProPublica. Considering the principles of security and the above questions, describe your threat model and how you would safely deliver the information.  
你是一名批评当地专制政府的记者，试图将你的报道发送给 ProPublica。考虑到安全原则和上述问题，描述你的威胁模型以及你如何安全地传递信息。

[](#security-building-blocks)Security Building Blocks   安全基础模块
==============================================================

[](#encryption)Encryption   加密
------------------------------

### [](#a-brief-introduction)A Brief Introduction   简要介绍

Hiding information from unauthorized users is a critical element of computer security. How can you make sure we can keep this information hidden in the case that we have to send it out into the open?  
隐藏信息免受未经授权用户的窥视是计算机安全的关键要素。如果必须将信息发送到公开环境，你如何确保信息能够保持隐藏？

Here’s where the power of **encryption** comes in. Encryption involves taking the information you want to hide, called the **plaintext**, and scrambling it into a format that can only be read with a key called the **ciphertext.**  
加密技术的力量就在这里。加密涉及将你想要隐藏的信息（称为明文）转换成一种只能通过特定密钥（称为密文）解密的格式。

### [](#types-of-encryption)Types of Encryption   加密类型

There are many different types of encryption algorithms, with different types of encryption keys, encryption speeds, security, and usefulness. **The most important quality of encryption algorithms is the fact that they are one-way**: it is easy to compute the ciphertext from the plaintext **(encrypt)** but extremely difficult to compute the plaintext from the ciphertext **(decrypt)** without the secret key.  
存在许多不同类型的加密算法，它们在加密密钥类型、加密速度、安全性以及实用性方面各不相同。加密算法最重要的特性是它们的单向性：从明文计算密文（加密）相对容易，但没有秘密密钥的情况下，从密文计算明文（解密）则极其困难。

There are two primary types of encryption: **symmetric key cryptography**, and asymmetric-key or **public-key cryptography**. The invention of public key cryptography was critical to many of the security features we take for granted today.  
有两种主要类型的加密：对称密钥密码学，以及非对称密钥或公钥密码学。公钥密码学的发明对于今天我们习以为常的许多安全特性至关重要。

For the purpose of this lab, we will be using several tools for performing encryption operations, among them **GnuPG**, a free implementation of the [OpenPGP](https://openpgp.org/) standard, and [**OpenSSL**](https://openssl.org), a library for implementing TLS, or Transport Layer Security.  
为完成本次实验，我们将使用几种工具来进行加密操作，其中包括 GnuPG，它是 OpenPGP 标准的免费实现，以及 OpenSSL，这是一个用于实现 TLS（传输层安全协议）的库。

#### [](#symmetric-key-cryptography)Symmetric Key Cryptography   对称密钥加密

Symmetric-key cryptography is named for the fact that **a single key both encrypts and decrypts a particular plaintext to ciphertext and vice-versa**. The most widely used method of symmetric crypto is AES, or _Advanced Encryption Standard_, which is certified and trusted by the US Government to encrypt information critical to national security. An easy way to visualize the basics of symmetric key crypto is by the XOR function: `a XOR b` is true if and only if a and b are different. Suppose our plaintext is the bitstring `100110100101`, and we want to apply a bitwise XOR with the bitstring `010010100010`, which is our key. The result of this operation is `110100000111`. If we XOR the resulting ciphertext with the key again, we get the original plaintext: `100110100101`. The XOR function is trivially reversible, but military-grade symmetric key cryptography algorithms are much more difficult to reverse. The security of symmetric-key crypto, therefore, is dependent on keeping the encryption key secret.  
对称密钥密码学之所以得名，是因为一个密钥既可以将特定的明文加密为密文，也可以将密文解密为明文。目前最广泛使用的对称加密方法是 AES（高级加密标准），它被美国政府认证并信任用于加密关系到国家安全的重要信息。一种简单的方式来可视化对称密钥密码学的基本原理是异或（XOR）函数： `a XOR b` 只有在 a 和 b 不同时才为真。假设我们的明文是位串 `100110100101` ，我们想要用位串 `010010100010` （即我们的密钥）进行位异或操作。该操作的结果是 `110100000111` 。如果我们再次将生成的密文与密钥进行异或操作，就能得到原始明文： `100110100101` 。异或函数可以轻易地被逆转，但军用级别的对称密钥密码学算法则要困难得多。因此，对称密钥密码学的安全性取决于保持加密密钥的机密性。

![alt text](/assets/images/labs/a9/XOR.GIF "xor in crypto")

**Symmetric key crypto is useful for almost everything, especially things that fall in these categories:  
对称密钥加密对于几乎所有事情都很有用，尤其是属于以下这些类别的事情：**

*   Encrypting data in transit (such as HTTPS)  
    加密传输中的数据（如 HTTPS）
*   Encrypting data at rest (like data stored on your phone)  
    对静态数据进行加密（如存储在你手机上的数据）

As an example, let’s explore how iPhones use encryption to keep your data safe:  
举个例子，让我们探讨一下 iPhone 如何利用加密来保护你的数据安全：

1.  The iPhone’s internal storage is encrypted with a set of AES keys stored on a chip inside the phone, generated at the factory.  
    iPhone 的内部存储使用一组 AES 密钥进行加密，这些密钥存储在手机内部的芯片上，是在工厂生成的。
2.  These keys are in turn encrypted with your PIN. Your PIN allows the phone to unlock the keys that allow it to decrypt the rest of the filesystem.  
    这些密钥随后会使用您的 PIN 进行加密。您的 PIN 允许手机解密那些能够解密文件系统其余部分的密钥。

This is how the iPhone is able to quickly wipe your data in the event your phone is stolen: the phone simply deletes the keys that are stored on the internal chip. Now, even with your PIN, there are no keys to decrypt and the encrypted data in the phone’s storage is, for all intents and purposes, irretrievable and indistinguishable from random data.  
iPhone 能够快速擦除你的数据，以防手机被盗，其原理是：手机只是删除存储在内部芯片上的密钥。现在，即使你设置了 PIN 码，也没有密钥可以用来解密，手机存储中的加密数据在实际上已无法恢复，且与随机数据无异。

#### [](#public-key-cryptography)Public Key Cryptography    公钥密码学

Unlike symmetric key cryptography, there are 2 keys in a public-key cryptosystem, the **public key** and the **private key**. As the name suggests, the public key is shared publicly, and this is the means by which other people encrypt data that’s meant for you. You use your private key to decrypt this data. As long as no one has your private key, anyone can use your public key to encrypt data and be assured that only you can decrypt it. This is a powerful expansion on the symmetric-key paradigm, as beyond encryption, it also allows for signatures and non-repudiation: a way for someone to verify that the person they are talking to is in fact the person they intend to be talking to, and for someone to prove (without the ability to deny it) that they are who they say they are.  
与对称密钥密码不同，公钥密码系统包含两个密钥：公钥和私钥。正如其名，公钥是公开的，其他人可以通过它来加密发给你的数据。你使用自己的私钥来解密这些数据。只要没有人拥有你的私钥，任何人都可以使用你的公钥来加密数据，并确信只有你能解密它。这在对称密钥范式的基础上是一个强大的扩展，因为除了加密之外，它还允许签名和不可抵赖性：一种方法，让某人能够验证他们正在交谈的人是否确实是他们所预期的那个人，并且让某人能够证明（无法否认）他们就是他们声称的身份。

Nowadays, public-key cryptography is synonymous with the **[RSA algorithm](https://en.wikipedia.org/wiki/RSA_(cryptosystem))**, which was one of the first proven dual-key schemes. (You will encounter the RSA algorithm in CS 70 and CS 161 if you plan to take those courses, or already have.) In short, the security of RSA depends on the theoretical difficulty of factoring large numbers on conventional computers. This is expected to continue to be a difficult problem until quantum computers become practical.  
如今，公钥密码学与 RSA 算法同义，RSA 是第一个被证明的双密钥方案之一。(如果你计划修读这些课程，或者已经修读过 CS 70 和 CS 161，你将会遇到 RSA 算法。) 简而言之，RSA 的安全性依赖于在传统计算机上对大数进行因数分解的理论难度。这一问题预计在量子计算机变得实用之前仍将是一个困难的问题。

Here’s a brief overview of how RSA public key crypto works:  
RSA 公钥密码的工作原理简要概述如下：

1.  The RSA algorithm, by means of some advanced mathematics (involving prime numbers and modular arithmetic), returns 3 numbers: a public exponent (aka key), a private exponent, and a modulus. The two keys work such that data encrypted with one key can only be decrypted with the other key.  
    RSA 算法通过一些高级数学方法（涉及质数和模运算）返回三个数字：一个公钥（也称为公钥指数），一个私钥指数，以及一个模数。这两个密钥的运作方式是，用其中一个密钥加密的数据只能用另一个密钥解密。
2.  In order to encrypt data, one performs modular exponentiation over the data using one of the exponents and the modulus.  
    为了加密数据，人们使用其中一个指数和模数对数据进行模幂运算。
3.  In order to decrypt data, one performs modular exponentation on the encrypted data with the partner key and modulus. In common use, one uses the larger exponent as the private key, which is used for decrypting data and creating signatures, and the smaller exponent as the public key, which is used for encryping data and verifying signatures.  
    为了解密数据，人们使用合作伙伴的密钥和模数对加密后的数据进行模幂运算。通常，较大的指数作为私钥，用于解密数据和创建签名，而较小的指数作为公钥，用于加密数据和验证签名。

In [lab a4](a4#generating-and-using-ssh-keys), one task was to generate an SSH key, using the command:  
在 a4 实验中，有一个任务是使用以下命令生成 SSH 密钥：

```
ssh-keygen -t rsa -b 4096 
```

This command would generate two files, `~/.ssh/id_rsa` and `~/.ssh/id_rsa.pub`. As the command suggests, this command generates a 4096-bit RSA key pair. You should be able to guess which file represents the public key and which one must therefore be the private key. In order to affect secure SSH logins using the RSA key, the user must first transfer the public key they wish to use to identify themselves to the server in advance. Then, once a session has been established between the server and the client, the server will encrypt a random number with the user’s public key and transmit it to the user. The user will then decrypt the value using their private key and return a hash of that value to the server, who can then hash the value themselves to determine if the user was able to successfully decrypt the random number, thus indicating posession of the matching secret key and serving as proof for authentication.  
此命令将生成两个文件， `~/.ssh/id_rsa` 和 `~/.ssh/id_rsa.pub` 。正如命令所暗示的，此命令生成一个 4096 位的 RSA 密钥对。你应该能够猜出哪个文件代表公钥，哪个文件因此必须是私钥。为了使用 RSA 密钥实现安全的 SSH 登录，用户必须首先将他们想要用来识别自己的公钥传输到服务器上。然后，一旦服务器和客户端之间建立了会话，服务器将使用用户的公钥加密一个随机数并将其发送给用户。用户随后使用他们的私钥解密该值，并将该值的哈希返回给服务器，服务器可以自己对这个值进行哈希处理，以确定用户是否成功解密了随机数，从而表明用户拥有匹配的私钥，并作为身份验证的证明。

[](#signatures-and-certificates)Signatures and Certificates   签名和证书
-------------------------------------------------------------------

Public key cryptography allows for a number of important security objects, including signatures and certificates for digital identity verification.  
公钥密码学允许实现多种重要的安全功能，包括用于数字身份验证的签名和证书。

Suppose you are an important public entity (maybe your pseudonym is Natoshi Sakamoto). You are in charge of an important project, hereafter known at Litcoin. You’d like to maintain anonymity, but still need to lead your project.  
假设你是重要的公共机构（也许你的假名是 Natoshi Sakamoto）。你负责一个重要的项目，以下简称 Litcoin。你希望保持匿名，但仍然需要领导你的项目。

How can your loyal followers know that statements supposedly made by Natoshi Sakamoto are actually from you?  
你的忠实追随者如何知道那些声称是 Natoshi Sakamoto 所说的话确实是出自你之口？

There’s a significant incentive to make false statements supposedly “from” Mr. Sakamoto, because each Litcoin is apparently worth a significant amount of real money, and some people stand to gain substantially if they are able to influence the direction of the project in their favor.  
有很强的动机去发布假消息，声称是“来自”Sakamoto 先生的，因为每枚 Litcoin 似乎都价值相当大的现实货币，而有些人如果能够影响项目的发展方向以符合自己的利益，就可能获得巨额收益。

You can avoid this by signing your messages: in the beginning, you would publish Natoshi’s public key, and thereafter, for every post you make, you would encrypt the content of the message using your (Natoshi’s) private key and post that along with your original message. Then, anyone who wants to verify that Natoshi (i.e. the owner of the private key corresponding to the public key that belongs to Natoshi) did in fact publish a particular message can simply use Natoshi’s public key to decrypt the encrypted signature and compare the content against the original message.  
你可以通过签名来避免这个问题：一开始，你会发布 Natoshi 的公钥，之后，对于你发布的每一条消息，你都会使用你的（Natoshi 的）私钥对消息内容进行加密，并将加密后的签名与原始消息一起发布。这样，任何想要验证 Natoshi（即私钥拥有者，该私钥对应于 Natoshi 的公钥）是否确实发布了某条消息的人，只需使用 Natoshi 的公钥解密加密后的签名，并将解密后的内容与原始消息进行比较即可。

Pretenders to Natoshi’s throne will be unable to sign their false statements such that they can be verified with Natoshi’s published public key because they don’t have Natoshi’s private key, and you can rest assured that no one will unduly influence your project in your name while you go into hiding from the IRS and DEA, unless they happen to have warehouses full of ASICs and lots of cheap electricity.  
那些觊觎 Natoshi 王位的冒名者将无法签署他们的虚假声明，使得这些声明能够用 Natoshi 发布的公钥进行验证，因为他们没有 Natoshi 的私钥。你可以放心，当你躲藏起来以逃避国税局和缉毒局的追查时，没有人会以你的名义不当影响你的项目，除非他们恰好拥有装满 ASIC 芯片的仓库和大量廉价电力。

**However, in this scheme, how do you prevent an adversary from publishing a fake public key and claiming to be you?** (they can make valid signatures against that fake public key) Somehow, you need to “bootstrap” trust: someone would need to verify your identity and publicly affirm that your public key actually corresponds to you.  
然而，在这个方案中，如何防止对手发布一个伪造的公钥并声称自己就是你？（他们可以针对该伪造的公钥生成有效的签名）某种方式，你需要“自举”信任：需要有人验证你的身份，并公开确认你的公钥确实与你对应。

We do this by means of **certificates**: a signed statement claiming that a particular public key does, in fact, belong to who it claims to belong to.  
我们通过证书来实现这一点：一份经过签名的声明，声称某个公钥确实属于它所声称的拥有者。

Who signs this certificate? A **certificate authority**, someone we trust to be responsible about verifying identities and issuing signatures.  
是谁签署这份证书？是证书颁发机构，即我们信任的能够负责任地验证身份并颁发签名的实体。

But how do we know which CAs to trust, and how can we trust that a CA that claims to be trustworthy actually is? They probably need a certificate as well. It sounds like it might be turtles all the way down; however, the chain does end somewhere: the so-called root of trust, the root CAs. These are the CAs whose certificates are pre-installed by browsers and operating systems and therefore intrinsically trusted, without any further certificates necessary. If a root CA signs your certificate, we assume they’ve done the due diligence necessary to be willing to risk their reputation by signing your certificate, and basically take their word for it. This model, known as the **Web of Trust**, is how network security works today.  
但是我们如何知道应该信任哪些证书颁发机构（CAs）？我们又如何信任那些声称可信的 CA 实际上确实可信？它们可能也需要一个证书。听起来这似乎是一个无限循环的问题；然而，这个链条最终会终止：所谓的信任根，也就是根证书颁发机构（root CAs）。这些 CA 的证书是预先安装在浏览器和操作系统中的，因此被内置于信任体系中，无需任何其他证书。如果一个根 CA 签署了你的证书，我们假设他们已经完成了必要的尽职调查，愿意冒着损害声誉的风险来签署你的证书，因此基本上接受他们的说法。这种模式被称为信任网络（Web of Trust），这是当今网络安全所采用的模型。

Unfortunately, it isn’t as reliable as we may have hoped: some CAs are scummy and will sign anything for enough money, resulting in valid certificates being issued for domains like microsoft.com and github.com to entities who are very obviously not Microsoft or GitHub.[1](#fn:badwosign) Furthermore, any entity with enough border control can force the installation of their own root certificates (e.g. the government of Kazakhstan[2](#fn:badkazakh)) and intercept any traffic by issuing their own bogus certificates for any domain.  
不幸的是，它并不像我们希望的那样可靠：一些证书颁发机构（CAs）很不靠谱，只要有钱就会签署任何证书，导致像 microsoft.com 和 github.com 这样的域名被颁发给明显不是微软或 GitHub 的实体。 [1](#fn:badwosign) 此外，任何拥有足够边境控制能力的实体都可以强制安装他们自己的根证书（例如哈萨克斯坦政府 [2](#fn:badkazakh) ），并通过对任何域名发布伪造证书来拦截所有流量。

![OCF has https](/assets/images/labs/a9/certificate.png)

You might not realize it, but **you use and rely on certificates and signatures every day.** Any time you see a green lock near the address bar of a website you visit, you are accesssing the site over a TLS or HTTPS connection, and the data being transferred between you and the website is encrypted. When your browser connects to the website’s server, it asks for the server’s public key in order to set up an encrypted connection, and the server’s certificate in order to verify its identity as the server authorized to serve the domain you have requested. Your browser then validates the public key by verifiying the signatures on the certificate. If someone is attempting to perform a man-in-the-middle attack on you, this certificate verification step will fail, because it should be unlikely that a trusted CA will have issued a signed cert for your domain to an entity other than you (unless you have the misfortune of living in Kazakhstan). You will get a very intrusive notification informing you of this fact, and it is a bad idea to ignore the certificate verification failure notification.  
你可能没有意识到，但你每天都在使用和依赖证书和签名。每次你在访问网站时看到地址栏附近有一个绿色的锁，就说明你正在通过 TLS 或 HTTPS 连接访问该网站，你与网站之间传输的数据是加密的。当你的浏览器连接到网站的服务器时，它会请求服务器的公钥以建立加密连接，并请求服务器的证书以验证其身份，确认该服务器是被授权提供你所请求域名的服务器。你的浏览器随后通过验证证书上的签名来确认公钥的有效性。如果有人试图对你进行中间人攻击，这个证书验证步骤将会失败，因为可信的证书颁发机构（CA）不太可能为你的域名签发证书给除你之外的实体（除非你不幸居住在哈萨克斯坦）。你将会收到一个非常侵入性的通知，告知你这一情况，而忽略证书验证失败的通知是不好的做法。

[](#hashing)Hashing   哈希 ing
----------------------------

There are many times where we have large amounts of data but need to operate on smaller representations of that data. For example, suppose you download a 1GB file from the internet. You want to make sure that the file you downloaded has not been modified on its way to you (after having taken the DeCal, you know that integrity is a critical goal of computer security). How could you figure out if the file has been changed? You could try downloading another copy to see if there’s a difference, but both copies could have been modified, and it takes a long time to download 1GB. You could use your knowledge of signatures to see if the source has provided a signature of the file you could verify, but the signature would have to be over the whole file[3](#fn:pkcsize) and this is even more expensive. What we really need is a deterministic way to generate fixed-length representations of arbitrary-length data. Luckily, mathematics has our back with functions known as hash functions. If you took CS 61B, you probably implemented a hash table at some point, and may be familiar with the concept. Here’s an example of hash functions at work. Enter the following commands on your student VM (i.e. `ssh username.decal.xcf.sh`) to calculate the SHA1[4](#fn:sha1) hash of a 40MB file.  
很多时候我们拥有大量数据，但需要对这些数据的较小表示形式进行操作。例如，假设你从互联网上下载了一个 1GB 的文件。你想要确保下载到的文件在传输过程中没有被修改（在学习了 DeCal 课程后，你知道完整性是计算机安全的重要目标之一）。你如何判断文件是否被更改过？你可以尝试下载另一个副本进行比较，但两个副本都可能被修改，而且下载 1GB 文件需要很长时间。你也可以利用你对签名的知识，查看源文件是否提供了你可以验证的文件签名，但签名必须覆盖整个文件 [3](#fn:pkcsize) ，这甚至更加耗时。我们真正需要的是一个确定性的方法，能够生成任意长度数据的固定长度表示。幸运的是，数学为我们提供了名为哈希函数的工具。如果你上过 CS 61B 课程，你可能在某个时候实现过哈希表，并对这个概念有所了解。下面是一个哈希函数实际应用的例子。 在你的学生虚拟机（即 `ssh username.decal.xcf.sh` ）上输入以下命令，以计算一个 40MB 文件的 SHA1 [4](#fn:sha1) 哈希值。

1.  `wget https://raw.githubusercontent.com/0xcf/decal-labs/master/a9/lab9.jpg`
2.  `sha1sum lab9.jpg`

You should see `685e925838358fdc162935588c6ee0aa5a5caed6`, a 40-digit string. As you can imagine, transferring this string is much easier than transferring a large, 100MB file, and because of the properties of the SHA-1 algorithm, you can be reasonably sure that this file, and only this file, will always hash to that particular value. This means, if you want to verify that the file you downloaded hasn’t been corrupted, you can simply compare the hashes (or more specifically, a signature over the hash) to ensure that the file you’ve downloaded hasn’t been tampered with.  
你应该看到 `685e925838358fdc162935588c6ee0aa5a5caed6` ，一个 40 位的字符串。正如你所想象的，传输这个字符串比传输一个大而达 100MB 的文件要容易得多，而且由于 SHA-1 算法的特性，你可以合理地确信只有这个文件会始终哈希到该特定值。这意味着，如果你想验证下载的文件是否未被损坏，只需比较哈希值（或更具体地说，对哈希的签名）即可确保下载的文件未被篡改。

[](#encryption-lab-activity)Encryption Lab Activity
===================================================

In this lab, we will be exploring the topics covered so far: symmetric encryption and public key cryptography, hashing, signatures, and certificates. Please make sure you are logged into the appropriate server over ssh prior to completing any of the following tasks.  
在这个实验中，我们将探讨到目前为止所涵盖的主题：对称加密和公钥密码学、哈希、签名和证书。请在完成以下任何任务之前，确保通过 ssh 登录到正确的服务器。

### [](#submission)Submission   提交

Remember to submit your answers to Gradescope as you complete the lab!  
记得在完成实验时将答案提交到 Gradescope！

### [](#introduction-making-ssh-keys)Introduction: Making SSH keys

We’ll begin our exploration of security building blocks by creating SSH keypairs for ourselves, in case you haven’t already. If you already have an SSH key on your personal computer that you would like to continue to use, or one on `tsunami`, feel free to skip these steps and proceed to task 1, but please make sure that you can use that key to log into your student VM.  
我们将从创建 SSH 密钥对开始，探索安全构建模块。如果你的个人电脑上已经有 SSH 密钥并且希望继续使用，或者在 `tsunami` 上有 SSH 密钥，可以跳过这些步骤，直接进行任务 1，但请确保你能够使用该密钥登录到你的学生虚拟机。

As mentioned earlier in this document, and in [lab a4](a4#generating-and-using-ssh-keys), here is how to create a 4096-bit RSA SSH keypair:  
如前面在本文件和 lab a4 中提到的，这里是创建一个 4096 位 RSA SSH 密钥对的方法：

1.  Log into `tsunami.ocf.berkeley.edu` with your OCF account credentials (potentially not the same as your decal VM credentials). It is important you do these steps on `tsunami` and not your laptop, an OCF desktop, or your decal VM.  
    登录到 `tsunami.ocf.berkeley.edu` ，使用你的 OCF 账户凭证（可能与你的 decal VM 凭证不同）。请确保你在 `tsunami` 上执行这些步骤，而不是在你的笔记本电脑、OCF 桌面或 decal VM 上。
2.  Check to make sure you do not have an existing SSH key:  
    请检查以确保你没有现有的 SSH 密钥：
    1.  Enter `ls -l ~/.ssh`  进入 `ls -l ~/.ssh`
    2.  If you see files like `id_rsa` or `id_rsa.pub` then you already have an SSH key.  
        如果你看到类似 `id_rsa` 或 `id_rsa.pub` 的文件，那么你已经拥有了一个 SSH 密钥。
3.  If no key is present, create a new key now by typing `ssh-keygen -t rsa -b 4096`, and hit enter when prompted.  
    如果没有密钥存在，请现在通过输入 `ssh-keygen -t rsa -b 4096` 来创建一个新的密钥，并在提示时按回车键。
4.  If you are asked to type a password to protect the key, you may choose to enter one, but for the purpose of this lab, it is advisable to skip doing so.  
    如果你被要求输入一个密码来保护该密钥，你可以选择输入一个，但为了本实验的目的，建议不要这样做。

You should now see two new files in the `~/.ssh/` directory. `id_rsa` is your SSH private key, and it should like the following:  
现在你应该在 `~/.ssh/` 目录中看到两个新文件。 `id_rsa` 是你的 SSH 私钥，应该看起来像下面这样：

```
$ cat ~/.ssh/id_rsa
-----BEGIN RSA PRIVATE KEY-----
<random characters>
-----END RSA PRIVATE KEY------ 
```

Please try to not lose or leak this private key. The public key, `id_rsa.pub`, should be much shorter and look like the following:  
请尽量不要丢失或泄露这个私钥。公钥， `id_rsa.pub` ，应该要短得多，并且看起来像下面这样：

```
$ cat ~/.ssh/id_rsa.pub
ssh-rsa <characters> <username>@<host> 
```

It is safe to transfer your RSA public key out in the open, whereas you should never do so with your private key. It should only be transferred over trusted and encrypted communication channels over machines you trust.

You’ve just created for yourself an RSA keypair suitable for SSH authentication.  
你刚刚为自己创建了一对适合 SSH 认证的 RSA 密钥。

For the remaining tasks in this lab, you will be tasked with figuring out the appropriate commands yourself, by Googling, reading man pages, etc. Hints will be given as footnotes as necessary. Please document the commands you use and be prepared to provide a rationale for why you believe those commands are correct.  
对于本实验中的剩余任务，您需要通过谷歌搜索、阅读 man 页面等方式自行找出适当的命令。必要时会给出提示作为脚注。请记录下您使用的命令，并准备好说明您认为这些命令正确的理由。

### [](#warm-up-task)Warm-up Task

Figure out how to use the SSH key you’ve created to log into your student VM.[5](#fn:ssh-copy-id)

**Perform the remaining tasks on your student VM unless otherwise specified.**

### [](#task-2---symmetric-encryption)Task 2 - Symmetric Encryption   任务 2 - 对称加密

In order to explore encryption, we’re going to be using the `gpg` command.  
为了探索加密，我们将使用 `gpg` 命令。

1.  Download[6](#fn:wget) the file named `q2.txt.gpg` from [github](https://raw.githubusercontent.com/0xcf/decal-labs/master/a9/q2.txt.gpg), decrypt it using `gpg` and the password `weak_password`. **What is the decrypted content?**  
    下载 [6](#fn:wget) 命令，从 github 获取名为 `q2.txt.gpg` 的文件，使用 `gpg` 和密码 `weak_password` 解密。解密后的内容是什么？
    
2.  Create a text file with your full name, username, and some random content in it. Then, use `gpg` to symmetrically encrypt this text file using the TWOFISH algorithm and a password of your choice. Make sure the resulting encrypted file is in ASCII-armored format[7](#fn:armor).  
    创建一个包含你全名、用户名和一些随机内容的文本文件。然后，使用 `gpg` 通过 TWOFISH 算法和你选择的密码对这个文本文件进行对称加密。确保生成的加密文件是 ASCII-armored 格式 [7](#fn:armor) 。
    
    **What command did you use to do this?  
    你使用了什么命令来完成这个操作？**
    

### [](#task-3---asymmetric-encryption)Task 3 - Asymmetric Encryption   任务 3 - 非对称加密

Remember to perform all the following steps on your student VM.  
请记得在你的学生虚拟机上执行以下所有步骤。

In this task, we will be using `gpg` to create a PGP key.  
在这个任务中，我们将使用 `gpg` 来创建一个 PGP 密钥。

Step 0: Figure out how to do make this key[8](#fn:ghgpg), and create a 4096-bit RSA/RSA PGP key.  
步骤 0：弄清楚如何生成这个密钥 [8](#fn:ghgpg) ，并创建一个 4096 位的 RSA/RSA PGP 密钥。

You might be wondering: didn’t we just create a 4096-bit RSA key for use in SSH? Why can’t we just use the same key for GPG? The short answer is that, while technically possible, it is [inadvisable](https://serverfault.com/questions/346958/how-do-i-import-a-rsa-ssh-key-into-gpg-as-the-primary-private-key) to do so for security reasons.  
你可能在想：我们不是刚刚为 SSH 创建了一个 4096 位的 RSA 密钥吗？为什么不能用同一个密钥用于 GPG 呢？简短的回答是，虽然技术上可行，但从安全角度考虑，这样做并不推荐。

1.  After creating the key, retrieve the key ID and put it in the checkoff form.  
    在创建密钥后，获取密钥 ID 并将其填入检查表中。
    
2.  Export the public portion of the key in ASCII armor format and figure out how to distribute it. For every subsequent step, make sure to do everything using ASCII armored output where applicable.  
    将密钥的公开部分导出为 ASCII armor 格式，并弄清楚如何分发它。在后续步骤中，确保在适用的情况下使用 ASCII armor 格式的输出。
    
3.  The DeCal staff have a [public key available](https://raw.githubusercontent.com/0xcf/decal-labs/master/a9/staff_public_key.gpg). Figure out how to import the key, and write down the key id.  
    DeCal 工作人员有一个公开密钥可用。弄清楚如何导入该密钥，并写下密钥 ID。
    
4.  The DeCal staff have uploaded a few [files](https://github.com/0xcf/decal-labs/tree/master/a9) called directions to help you figure out how to get to the OCF. However, we suspect nefarious elements may be trying to modify the file in an effort to divert students to their location. Figure out if the file has been modified. (**Hint:** Check the signature!)  
    DeCal 工作人员上传了一些名为 directions 的文件来帮助你找到前往 OCF 的路。然而，我们怀疑某些不良分子可能试图修改该文件，以引导学生前往他们的位置。弄清楚该文件是否已被修改。（提示：检查签名！）
    
5.  Where is your private key located? Write down the location where you expect the private key to be.  
    你的私钥位于何处？写下你预期私钥所在的路径。
    

We will return to encryption later in the lab.  
我们将在实验的后面部分再回到加密。

### [](#task-4---hashing)Task 4 - Hashing

Fortunately all the common hashing algorithms have been implemented for us by various libraries. For this lab, please compute the requested hashes by any method you deem appropriate. It would be advisable to do these on an OCF desktop, or on your student VMs, but _not_ on tsunami, because this leaves around a large file that takes up extra space if not cleaned up.  
幸运的是，各种库已经为我们实现了所有常见的哈希算法。对于本次实验，请使用你认为合适的方法计算所需的哈希值。建议在 OCF 桌面或你的学生虚拟机上进行这些操作，而不是在 tsunami 上，因为这会留下一个占用额外空间的大文件，如果不清理的话。

1.  Download a copy of the CentOS 7 NetInstall ISO from the [OCF mirrors](http://mirrors.ocf.berkeley.edu/centos/7/isos/x86_64/) and verify that its SHA256 hash is correct. Also compute its SHA1 and MD5 hashes.  
    下载一份 CentOS 7 NetInstall ISO 镜像文件，并从 OCF 镜像站点验证其 SHA256 哈希值是否正确。同时计算其 SHA1 和 MD5 哈希值。
    
2.  You may have noticed that the CentOS project provides a signature over the hashes it provides (the .asc file in the same directory as above). Briefly explain why this is a good, efficient security measure, knowing what you do about how signatures, hashing, and malicious attempts at file corruption work.  
    你可能已经注意到 CentOS 项目在其提供的哈希值上提供了一个签名（如上文同一目录中的.asc 文件）。简要解释为什么这是一个良好的、高效的安全部署，考虑到你对签名、哈希以及恶意文件破坏尝试的了解。
    

[](#file-security)File Security    文件安全
---------------------------------------

Now that you have some experience with the primitives of encryption, let’s cover some practical topics in securing files on UNIX-based systems.  
现在你已经对加密的基本原理有一些经验，让我们来探讨一些在 UNIX 系统上保护文件的实际主题。

### [](#file-security-and-permissions)File Security and Permissions   文件安全与权限

The base layer in the UNIX security hierarchy is file permissions. Every file and process is owned by a user (and group), and by default, only the user/group that owns the file can edit it. You can see this by typing `ls -la` in your terminal:  
UNIX 安全体系的基础层是文件权限。每个文件和进程都归属于一个用户（和一个组），默认情况下，只有拥有该文件的用户/组才能对其进行编辑。你可以在终端中输入 `ls -la` 来查看这一点：

```
admin@staff:~$ ls -la
total 104
drwxr-xr-x 9 admin admin  4096 Oct  3 13:01 .
drwxr-xr-x 5 root  root   4096 Oct  2 16:49 ..
drwxr-xr-x 2 admin admin  4096 Sep 21 21:11 .augeas
-rw------- 1 admin admin 27932 Oct  6 14:05 .bash_history
-rw-r--r-- 1 admin admin   220 May 15 12:45 .bash_logout
-rw-r--r-- 1 admin admin  3526 May 15 12:45 .bashrc
drwx------ 3 admin admin  4096 Sep 17 12:02 .config
drwxr-xr-x 4 admin admin  4096 Oct  3 12:47 .gem
drwxr-xr-x 2 admin admin  4096 Oct  3 12:46 .nano
-rw-r--r-- 1 admin admin   675 May 15 12:45 .profile
drwxr-xr-x 4 admin admin  4096 Sep 17 14:23 .puppet
drwx------ 2 admin admin  4096 Sep 17 12:09 .ssh
drwxr-xr-x 3 admin admin  4096 Oct  3 12:38 test
-rw------- 1 admin admin 21980 Oct  2 16:42 .viminfo 
```

The first column, e.g. `-rw-------`, is the read/write/execute permissions for the file. The third and fourth columns are the user and group that own the file.  
第一列，例如 `-rw-------` ，是文件的读/写/执行权限。第三和第四列是文件的所有者和所属组。

Let’s break down the permissions seen in the example above. You’ll notice that there are 10 entries: they either start with `d` for directory or `-` for regular files[9](#fn:firstpermflag), the the remaining 9 entries are split into 3 groups of 3 permission each: `r`ead, `w`rite, and e`x`ecute for `u`user, `g`roup, and `o`ther. That means `-rw-------` refers to a regular file, where the owner can read and write but not execute the file, and no one else can either read, write, nor execute the file. On the other hand, the permissions on the `test` entry (`drwxr-xr-x`) indicate that it is a directory, everyone can enter the directory (`d--x--x--x`), anyone can list files inside the directory (`dr--r--r--`), but only the owning user can write files inside the directory (`d-w-------`). A key point to remember is the difference between execute permissions on files vs. directories: on directories, it refers to the ability to enter the directory.  
让我们分析上面示例中看到的权限。你会注意到有 10 个条目：它们要么以 `d` 开头表示目录，要么以 `-` 开头表示普通文件 [9](#fn:firstpermflag) ，剩下的 9 个条目分为 3 组，每组 3 个权限： `r` read， `w` write，和 `x` execute，分别对应 `u` user， `g` group，和 `o` other。这意味着 `-rw-------` 指的是一个普通文件，文件所有者可以读和写，但不能执行该文件，而其他人则既不能读，也不能写，也不能执行该文件。另一方面， `test` 条目（ `drwxr-xr-x` ）的权限表示这是一个目录，每个人都可以进入该目录（ `d--x--x--x` ），任何人都可以列出目录中的文件（ `dr--r--r--` ），但只有文件所有者才能在目录中写入文件（ `d-w-------` ）。一个关键点是记住文件和目录上的执行权限的区别：在目录上，执行权限指的是进入目录的能力。

![alt test](https://www.comentum.com/images/permissions.jpg)

The kernel enforces file permissions, preventing running programs from reading or modifying files they aren’t allowed to, and preventing users from executing programs that they don’t have access or permission for. This is important for a variety of reasons. For example, certain UNIX user accounts have their account information stored in a file called `/etc/passwd` and password hashes in `/etc/shadow`. These files are owned by `root`, preventing regular users from reading or changing this information without permission.  
内核强制执行文件权限，防止正在运行的程序读取或修改其无权访问的文件，同时防止用户执行其没有访问权限或权限的程序。这在很多方面都很重要。例如，某些 UNIX 用户账户的账户信息存储在一个名为 `/etc/passwd` 的文件中，密码哈希存储在 `/etc/shadow` 中。这些文件由 `root` 所有，防止普通用户在未获得许可的情况下读取或更改这些信息。

```
❯ ls -la /etc/shadow
-rw------- 1 root root 861 Oct  9 02:22 /etc/shadow 
```

This highlights a common security issue in UNIX: programs running as the `root` user. When a program is started, it inherits its user and group IDs from its parent process, and keeps them unless it manually drops permissions. If you start a program as the root user, because, for example, it requires deeper system access, a vulnerability in the program means that an attacker can interact with your computer as the root user. This is a common problem in misconfigured webservers, where a server running as root with a directory traversal vulnerability might allow an attacker to read secret credentials stored on the server’s filesystem.  
这突显了 UNIX 系统中一个常见的安全问题：以 `root` 用户身份运行的程序。当程序启动时，它会从父进程继承其用户和组 ID，并在没有手动降低权限的情况下一直保留这些 ID。如果你以 root 用户身份启动一个程序，例如因为它需要更深层次的系统访问权限，那么程序中的漏洞意味着攻击者可以以 root 用户身份与你的计算机进行交互。这在配置错误的 Web 服务器中尤为常见，例如，如果服务器以 root 身份运行并且存在目录遍历漏洞，攻击者可能可以读取存储在服务器文件系统中的秘密凭证。

The moral of this story is tied to the principle of least privilege: wherever possible, only give the minimum amount of permission or privilege possible. If a program doesn’t need root credentials, don’t run it as a privileged user. If a file has sensitive content, don’t make it world-readable.  
这个故事的寓意与最小特权原则有关：在可能的情况下，只给予最低限度的权限或特权。如果一个程序不需要 root 权限，就不要以特权用户身份运行它。如果一个文件包含敏感内容，就不要让它对所有人可读。

How do you change permissions? There are two primary commands for doing so: `chmod` and `chown`. `chmod` changes the file mode, i.e. permissions, and an example of its syntax follows:  
如何更改权限？有两个主要命令可以实现这一点： `chmod` 和 `chown` 。 `chmod` 用于更改文件模式，即权限，其语法示例如下：

```
$ ls -la ~/
drwxr-xr-x 3 admin admin  4096 Oct  3 12:38 test
$ chmod 644 test
$ ls -la
drw-r--r-- 3 admin admin  4096 Oct  3 12:38 test
$ chmod u+x test
drwxr--r-- 3 admin admin  4096 Oct  3 12:38 test
$ chmod 000 test
d--------- 3 admin admin  4096 Oct  3 12:38 test
$ chmod +r test
dr--r--r-- 3 admin admin  4096 Oct  3 12:38 test 
```

`chmod` accepts file permissions in octal notation, which is the following:  
`chmod` 接受八进制表示的文件权限，其格式如下：

<table><thead><tr><th>#</th><th data-imt_insert_failed="1">rwx</th></tr></thead><tbody><tr><td>7</td><td data-imt_insert_failed="1">rwx</td></tr><tr><td>6</td><td data-imt_insert_failed="1">rw-</td></tr><tr><td>5</td><td data-imt_insert_failed="1">r-x</td></tr><tr><td>4</td><td data-imt_insert_failed="1">r–</td></tr><tr><td>3</td><td data-imt_insert_failed="1">-wx</td></tr><tr><td>2</td><td data-imt_insert_failed="1">-w-</td></tr><tr><td>1</td><td data-imt_insert_failed="1">–x</td></tr><tr><td>0</td><td data-imt_insert_failed="1">—</td></tr></tbody></table>

`chown` on the other hand changes the owner and group of a file. For example, suppose a file `instructions.txt` is created in your home directory (`~you`) by user `staff` with permissions `-rw-------`. You need to read this file to follow the instructions, but the `staff` user did not make the file world-readable so you could open it. In order to read it, you may first want to `chown` the file to yourself, by running `chown you:you instructions.txt`. This would change the file’s owner and owning group, previously “staff” and “staff”, to you. The basic syntax of `chown` is fairly simple:  
另一方面， `chown` 用于更改文件的所有者和所属组。例如，假设一个文件 `instructions.txt` 是由用户 `staff` 在你的主目录（ `~you` ）中创建的，权限为 `-rw-------` 。你需要读取这个文件以遵循指示，但用户 `staff` 没有将文件设置为全局可读，因此你无法打开它。为了读取它，你可能首先想通过运行 `chown you:you instructions.txt` 将文件的所有权转给自己。这会将文件之前属于“staff”组的所有者和所属组更改为你。 `chown` 的基本语法相当简单：

```
chown [-R] [user]:[group] PATH 
```

where `PATH` is the file or directory whose ownership you wish to modify, and `-R` means ‘recursive.’  
其中 `PATH` 是您希望修改所有权的文件或目录， `-R` 表示“递归”。

Making sure that files are only accessible to those who should be allowed to do so, and that vulnerable programs are not given too many permissions, is a critical part of maintaining system security on Linux.  
确保文件仅对应该允许访问的人可访问，并且易受攻击的程序不会被赋予过多权限，是维护 Linux 系统安全的关键部分。

### [](#task-5---file-security)Task 5 - File Security   任务 5 - 文件安全

Lets practice using the commands and concepts covered above.  
让我们练习使用上面介绍的命令和概念。

Please answer the following questions with regards to some arbitary files, and make note of what commands you would use for checkoff. It’s recommended to play around with those commands on your own.  
请回答以下问题，这些问题涉及一些任意文件，并记录下你用于检查的命令。建议你自行尝试这些命令以加深理解。

Let’s say you have a file:  
假设你有一个文件：

1.  How would you make that file readable to you?  
    你如何让这个文件对你可读？
2.  What if the file is very important, as it contains decal secrets. Use `chmod` and `chown` to make the file readable only to you.  
    如果这个文件非常重要，因为它包含 decal 的机密信息。使用 `chmod` 和 `chown` 让这个文件只能被你读取。
3.  Lets say we have an even more important file. Only root should be able to read this file, and no one should be able to edit it.  
    假设我们有一个更加重要的文件。只有 root 应该能够读取这个文件，而且没有人应该能够编辑它。
4.  Lets say `file4.txt` and `file5.txt` are files are owned by another user. Choose any method to make the files readable to you and unreadable to the previous owner.  
    假设 `file4.txt` 和 `file5.txt` 是其他用户拥有的文件。选择任何方法使这些文件对你可读，而对之前的拥有者不可读。
5.  Lets say we have two files, `filea.txt` and `fileb.txt`: provide a strategy to make these files readable only to you and the `decal` user, and no one else.  
    假设我们有两个文件， `filea.txt` 和 `fileb.txt` ：请提供一个策略，使这些文件只能被你和 `decal` 用户读取，而其他人无法访问。

[](#network-security)Network Security   网络安全
--------------------------------------------

On UNIX based operating systems, the network is the most common method of gaining unauthorized access to a machine. Why this is the case should be obvious: the network allows one to interact with a machine without needing any physical presence. Anyone on the same network as your machine can connect, and therefore attack, your machine. On the internet, this means the whole world can attack your machine. This introduces a number of security considerations when running networked applications.  
在基于 UNIX 的操作系统中，网络是获得机器未经授权访问的最常见方式。为什么会这样显而易见：网络允许一个人在不需要任何物理存在的情况下与机器进行交互。任何与你的机器在同一网络上的人都可以连接，并因此攻击你的机器。在互联网上，这意味着全世界都可以攻击你的机器。这在运行网络应用程序时引入了诸多安全考虑因素。

The first step in securing machines connected to a network, or the open internet, is to make it as hard as possible for attackers to get to your machine. As you may recall from the networking lecture, processes interact with one another over a network by means of sockets and ports. Many protocols listen on well-known ports, such as SSH on port 22, HTTP on port 80, FTP on port 21, etc. We use firewalls and similar software to prevent unauthorized users from connecting to your machine.  
在网络连接的机器或开放互联网上的第一步骤是让攻击者尽可能难以接触到你的机器。正如你在网络课程中可能记得的，进程通过套接字和端口在网络中进行交互。许多协议监听众所周知的端口，例如 SSH 在端口 22，HTTP 在端口 80，FTP 在端口 21 等。我们使用防火墙和其他类似的软件来防止未经授权的用户连接到你的机器。

The second step in maintaining network security is to audit which programs are running on your server, and to design your network to reduce the attack vectors on any single machine by separating concerns as much as reasonably possible. For example, it may not be a good idea to run a critical database server and a potentially insecure webserver on the same machine, as a vulnerability in one could easily lead to the compromising of the other. At the very least, all these programs should be run as minimally privileged users, and never as `root` unless necessary, like `sshd`.  
维护网络安全的第二步是审查你的服务器上运行哪些程序，并设计你的网络以尽可能减少对任何单个机器的攻击面，通过将不同的职责分离。例如，将关键的数据库服务器和可能不安全的网页服务器运行在同一台机器上可能不是一个好主意，因为一个中的漏洞可能会轻易导致另一个被攻破。至少，所有这些程序都应该以最小特权的用户身份运行，除非必要，否则绝不要以 `root` 身份运行，比如 `sshd` 。

Finally, the hardest part of online security is auditing your own code. Ultimately, most security vulnerabilities arise from code you need to run to affect your business functions. For example, the popular CMS software WordPress, which is estimated to power nearly a quarter of all websites, has historically been extremely vulnerable to security bugs, especially because people like to install WordPress plugins, often written by amateur coders, which are even less secure. Unfortunately, the only real defense against one’s own imperfection as a programmer is to be vigilant about monitoring one’s programs for vulnerabilities and servers for intrusion, and following good defensive programming practices on the road towards the impossible goal of secure, bug-free programs.  
最后，在线安全最难的部分是审查自己的代码。归根结底，大多数安全漏洞都来自于你需要运行以影响业务功能的代码。例如，流行的 CMS 软件 WordPress，据估计为近四分之一的网站提供支持，历史上一直非常容易受到安全漏洞的攻击，尤其是因为人们喜欢安装 WordPress 插件，这些插件通常由业余程序员编写，安全性更差。不幸的是，防止自己作为程序员的不完美性的唯一真正方法，就是保持警惕，监控自己的程序是否存在漏洞，服务器是否被入侵，并在通往不可能实现的目标——完全安全、无漏洞的程序的道路上，遵循良好的防御性编程实践。

### [](#task-6---network-security-lab-activity)Task 6 - Network Security Lab Activity   任务 6 - 网络安全实验室活动

1.  Make a list of all the services running on your student VM that are accessible from the public internet, what user they are running as, and what port they are listening on. You may find tools such as `netstat` and `ps` to be helpful. You may also want to point `nmap -A` at your VM from another machine, such as `tsunami`.  
    列出你学生虚拟机上所有可以从公共互联网访问的服务，包括它们以什么用户身份运行，以及它们监听的端口。你可能会发现工具如 `netstat` 和 `ps` 很有帮助。你也可以从另一台机器上将 `nmap -A` 指向你的虚拟机，例如 `tsunami` 。
    
2.  Use `less` and `grep` to open up and search your SSH login log file, located in `/var/log/auth.log`. Besides yourself, is anyone trying to log in? Who and why if there is?  
    使用 `less` 和 `grep` 来打开并搜索你的 SSH 登录日志文件，该文件位于 `/var/log/auth.log` 。除了你自己之外，是否还有其他人尝试登录？如果有，是谁？为什么？
    

* * *

[](#optional-task-letsencrypt-on-an-nginx-instance)Optional Task: Letsencrypt on an nginx instance!   可选任务：在 nginx 实例上使用 Let'sencrypt!
--------------------------------------------------------------------------------------------------------------------------------------

**This task is somewhat involved and completely optional.** With that said, being able to use `certbot` to provision certificates is an invaluable skill that you will likely need when setting up your own webservers. If you ever need to do this in the future, you know where to find a guide! :)  
这个任务有些复杂，而且是完全可选的。不过，能够使用 `certbot` 来申请证书是一项非常有价值的技能，当你自己设置 web 服务器时很可能需要用到。如果你将来需要这样做，就知道在哪里可以找到指南了！:)

In this task, we’re going to set up one of the most common uses of certificates and signatures: HTTPS. As you might have realized, you are now in possession of a website and webserver, (<you>.decal.xcf.sh), at least for the time being. Perform the following tasks on your student VM.  
在这个任务中，我们将设置证书和签名最常见的用途之一：HTTPS。正如你可能已经意识到的，你现在拥有一个网站和 web 服务器，(<you>.decal.xcf.sh)，至少暂时如此。请在你的学生虚拟机上执行以下任务。

Suppose you want to start hosting files on your site for other people to visit and see.  
假设你想开始在你的网站上托管文件，让其他人访问和查看。

First, install the `nginx` package to get a web server, and then customize the file at `/var/www/html/index.html` to reflect your indiviuality. You can see this new file at `<you>.decal.xcf.sh` in a web browser.  
首先，安装 `nginx` 包以获取一个 Web 服务器，然后自定义 `/var/www/html/index.html` 文件以反映你的个性。你可以通过 `<you>.decal.xcf.sh` 在网页浏览器中查看这个新文件。

Now that you have a website, you decide that, as a good internet citizen, you want to protect your visitors from the prying eyes of the government, by setting up HTTPS. You know already that you will need a public key and a certificate signed by a trusted root CA in order to do this. How do you go about getting one? Searching the internet, you find a wonderful project called [Let’s Encrypt](https://letsencrypt.org) that provides free, signed certificates. Let’s go about acquiring one.  
现在你已经有了一个网站，你决定作为一位良好的互联网公民，通过设置 HTTPS 来保护你的访客免受政府的窥视。你知道为了做到这一点，你需要一个公钥和一个由受信任的根 CA 签名的证书。你该如何获取这些呢？在互联网上搜索后，你发现了一个名为 Let’s Encrypt 的绝佳项目，它提供免费且已签名的证书。让我们开始获取一个证书。

For the purpose of this lab, we will be performing all the required steps manually instead of using the automated tools for educational purposes.  
为了本实验的目的，我们将手动执行所有必要的步骤，而不是使用自动化工具，以便进行教育学习。

1.  The first step in acquiring a signed certificate is to generate the public and private key that the certificate will validate. We can do so by using the `openssl` command.  
    获取已签名证书的第一步是生成证书将验证的公钥和私钥。我们可以使用 `openssl` 命令来完成此操作。
    
    `$ openssl genrsa 4096 > domain.key`
    
    As you probably guessed, we just created another 4096-bit RSA keypair, this time, in PEM format. Both the public and private keys are encoded in the file `domain.key`, which you can see using `cat`.  
    正如你可能已经猜到的，我们刚刚创建了另一个 4096 位的 RSA 密钥对，这次是以 PEM 格式保存的。公钥和私钥都编码在文件 `domain.key` 中，你可以使用 `cat` 查看。
    
2.  For Let’s Encrypt, we’ll also need an another key for our Lets Encrypt account. Repeat the command from above, but this time, output to a file called `account.key`.[10](#fn:account-key) Let’s also export the public portion of this key as you’ll need it soon when you authenticate to LetsEncrypt.  
    对于 Let’s Encrypt，我们还需要另一个密钥用于我们的 Let’s Encrypt 账户。重复上面的命令，但这次将输出保存到名为 `account.key` 的文件中。 [10](#fn:account-key) 同时，我们也需要导出这个密钥的公钥部分，因为稍后在认证 Let’s Encrypt 时会用到。
    
    `$ openssl rsa -in account.key -pubout > account.pub`
    
3.  Now that we have a keypair that we want signed, (`domain.key`), let’s generate a “Certificate Signing Request” for it. This will be what you send to Let’s Encrypt in order to get a certificate for your public key. It contains the metadata necessary for LE to issue a certificate.  
    现在我们已经有了一个想要签名的密钥对（ `domain.key` ），让我们为它生成一个“证书签名请求”。这将是您发送给 Let’s Encrypt 以获取您公钥证书的内容。它包含 Let’s Encrypt 颁发证书所需的元数据。
    
    `$ openssl req -new -sha256 -key domain.key -subj "/C=US/ST=CA/L=UC Berkeley/O=Open Computing Facility/emailAddress=<you>@ocf.berkeley.edu/CN=<you>.decal.xcf.sh" -out csr.pem -outform pem`
    
    You’ll find a file in your directory called `csr.pem` that starts with `BEGIN CERTIFICATE REQUEST`. What did you just do? You asked for a `-new` certificate signing `req`uest for the public `-key` in `domain.key`, using `-sha256` for hashing. You are <you>@something from the OCF at UC Berkeley, CA, US, and you’re making this request for the domain name (aka `C`ommon `N`ame) <you>.decal.xcf.sh. You’d also like the CSR to be in `pem` format.  
    你将在目录中发现一个名为 `csr.pem` 的文件，它以 `BEGIN CERTIFICATE REQUEST` 开头。你刚刚做了什么？你为公共 `-key` in `domain.key` 请求了一个 `-new` 证书签名 `req` uest，使用 `-sha256` 进行哈希。你是@something 来自加州大学伯克利分校的 OCF，你为域名（即 `C` ommon `N` ame）.decal.xcf.sh 发出了这个请求。你希望 CSR 以 `pem` 格式呈现。
    
    Now, we have all the data we need to actually get a certificate. Let’s submit the request to Let’s Encrypt.  
    现在，我们已经拥有了实际获取证书所需的所有数据。让我们将请求提交给 Let’s Encrypt。
    
4.  Install the `certbot` utility and do the following:  
    安装 `certbot` 工具并执行以下操作：
    
    Enter the following command at a terminal:  
    在终端中输入以下命令：
    
    ```
     sudo certbot certonly \
          --authenticator manual \
          --text \
          --email <you>@email.address \
          --csr csr.pem 
    ```
    
    You will be asked to answer some questions, read them and type “Y” when prompted. On the last prompt, you will be asked to make a file available in the root directory of the web server, either by copying the file yourself or by executing the Python code provided by the `certbot` program. It is recommended to use the Python code in a new terminal window. Here is what the code should look like: (_do not copy and paste this_, use the code provided by `certbot`)  
    你将被要求回答一些问题，阅读问题并被提示时输入“Y”。在最后一个提示中，你将被要求在 Web 服务器的根目录下提供一个文件，可以通过自己复制文件或执行 `certbot` 程序提供的 Python 代码来完成。建议在新的终端窗口中使用 Python 代码。代码应该如下所示：（不要复制粘贴此代码，请使用 `certbot` 提供的代码）
    
    ```
     # mkdir -p /tmp/certbot/public_html/.well-known/acme-challenge
     # cd /tmp/certbot/public_html
     # printf "%s" <random> > .well-known/acme-challenge/<random>
       # run only once per server:
     # sudo $(command -v python2 || command -v python2.7 || command -v python2.6) -c \
        "import BaseHTTPServer, SimpleHTTPServer; \
        s = BaseHTTPServer.HTTPServer(('', 80), SimpleHTTPServer.SimpleHTTPRequestHandler); \
        s.serve_forever()" 
    ```
    
    After this, the `certbot` client will verify that the domain is legit and issue a certificate, writing it to your current directory, likely as `0000_chain.pem`.  
    之后， `certbot` 客户端将验证域是否合法，并颁发证书，将其写入您的当前目录，通常为 `0000_chain.pem` 。
    
    This `.pem` file is your signed certificate! Once you set it up with a web server, people will be able to securely browse your website.  
    这个 `.pem` 文件是你的签名证书！一旦你将其配置到 Web 服务器上，人们就可以安全地浏览你的网站。
    
    Let’s configure the webserver to use your new certificate to secure requests.  
    让我们配置 Web 服务器以使用你的新证书来安全地处理请求。
    
5.  Make sure that `nginx` is installed, otherwise you won’t be able to view your website.  
    确保 `nginx` 已安装，否则你将无法查看你的网站。
    
    `nginx` stores its configuration information in `/etc/nginx`. From the [decal-labs](https://github.com/0xcf/decal-labs/) repository, add the file `a9/nginx-a9.conf` to the `/etc/nginx/sites-enabled/` directory.  
    `nginx` 将其配置信息存储在 `/etc/nginx` 中。从 decal-labs 仓库中，将文件 `a9/nginx-a9.conf` 添加到 `/etc/nginx/sites-enabled/` 目录中。
    
6.  Edit `nginx-a9.conf` using your preferred text editor and fix it according to the instructions contained therein.  
    使用你偏好的文本编辑器编辑 `nginx-a9.conf` ，并根据其中的说明进行修复。
    
7.  Rename `nginx-lab9.conf` to `default` and reload nginx.[11](#fn:reloadnginx)  
    将 `nginx-lab9.conf` 重命名为 `default` 并重新加载 nginx。 [11](#fn:reloadnginx)
    

Now, if you visit `https://<you>.decal.xcf.sh`, you should see the green lock in the address bar indicating that your website is secured with HTTPS!  
现在，如果你访问 `https://<you>.decal.xcf.sh` ，你应该在地址栏看到绿色的锁，这表明你的网站已经通过 HTTPS 进行了安全保护！

[](#footnotes)Footnotes   注释
============================

1.  https://thehackernews.com/2016/08/github-ssl-certificate.html [↩](#fnref:badwosign)
    
2.  https://news.ycombinator.com/item?id=10663843 [↩](#fnref:badkazakh)
    
3.  limitations on the size of data that public key crypto can operate over render this option nontrivial and difficult to implement. [↩](#fnref:pkcsize)  
    公钥加密所能处理的数据大小的限制使得这一选项并不简单且难以实现。 ↩
    
4.  https://en.wikipedia.org/wiki/SHA1 [↩](#fnref:sha1)
    
5.  hint: look at the man page `ssh-copy-id` command. [↩](#fnref:ssh-copy-id)  
    提示：查看 `ssh-copy-id` 命令的 man 页面。 ↩
    
6.  use of wget is recommended [↩](#fnref:wget)  
    建议使用 wget ↩
    
7.  see the `--armor` option [↩](#fnref:armor)  
    参见 `--armor` 选项 ↩
    
8.  GitHub has [excellent documentation](https://help.github.com/articles/generating-a-new-gpg-key/) on creating all kinds of cryptographic keys. [↩](#fnref:ghgpg)  
    GitHub 对创建各种加密密钥有非常好的文档说明。 ↩
    
9.  There are others too, like ‘l’ for symbolic link [↩](#fnref:firstpermflag)  
    还有其他的，比如 ‘l’ 表示符号链接 ↩
    
10.  `$ openssl genrsa 4096 > account.key` [↩](#fnref:account-key)
    
11.  recall lab 6: `systemctl reload nginx` [↩](#fnref:reloadnginx)  
    回顾实验 6： `systemctl reload nginx` ↩
    

* * *

[![](/assets/images/digitalocean.png)](https://www.digitalocean.com) With great appreciation to [DigitalOcean](https://www.digitalocean.com) for sponsoring the VMs used in both tracks of the DeCal

 [![](/assets/images/linode.png)](https://www.linode.com) Huge thanks to [Linode](https://www.linode.com) for sponsoring the equipment used to record digital lectures for the Decal

[![Hosted by the OCF](https://www.ocf.berkeley.edu/hosting-logos/ocf-hosted-penguin.svg)](https://www.ocf.berkeley.edu) Copyright © 2017-2021 [ Open Computing Facility ](https://www.ocf.berkeley.edu) and [eXperimental Computing Facility](https://xcf.berkeley.edu) 

This website and its course materials are licensed under the terms of the [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/) License. [Source Code](https://github.com/0xcf/decal-web/) available on GitHub
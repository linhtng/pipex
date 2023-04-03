# pipex
<h1 align="center">
	📖 Pipex
</h1>

<p align="center">
	<b><i>Reproduction of shell pipes</i></b><br>
</p>

<p align="center">
	<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/linhtng/pipex?color=lightblue" />
	<img alt="Number of lines of code" src="https://img.shields.io/tokei/lines/github/linhtng/pipex?color=critical" />
	<img alt="Code language count" src="https://img.shields.io/github/languages/count/linhtng/pipex?color=yellow" />
	<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/linhtng/pipex?color=blue" />
	<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/linhtng/pipex?color=green" />
</p>
<h3 align="center">
	<a href="#%EF%B8%8F-about">About</a>
	<span> · </span>
	<a href="#%EF%B8%8F-usage">Usage</a>
	<span> · </span>
</h3>

---

## 💡 About the project

> _Pipex is a project that re-creates in C the way two commands are piped together via `|` in the shell

For more detailed information, look at the 
[pipex_subject.pdf](https://github.com/linhtng/pipex/files/11137478/pipex_subject.pdf)


## 🛠️ Usage
### Mandatory
```shell
./pipex file1 cmd1 cmd2 file2
```
* Execution of ``pipex`` should be similar to the next shell command:

```shell
< file1 cmd1 | cmd2 > file2
```

``file1``, ``file2`` - filenames

``cmd1``, ``cmd2`` - shell commands with their parameters

<br>

* Handling errors must be like in shell

### Bonus part

* ``pipex`` should handle multiple pipes:

```shell
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
```

is equivalent to the shell command:

```shell
< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2
```

<br>

* ``pipex`` should support `«` and `»` when the first parameter is `here_doc`:

```shell
./pipex here_doc LIMITER cmd1 cmd2 file
```
is equivalent to the shell command:

```shell
cmd1 << LIMITER | cmd2 >> file
```

---

<a name="installation"></a>
## Installation

* Clone this repository:

```shell
https://github.com/linhtng/pipex.git
```

* Go to the project folder:

```shell
cd pipex
```

* To compile mandatory part:

```shell
make
```

* To compile bonus part:

```shell
make bonus
```

* After compilation just run ``pipex`` with your parameters.

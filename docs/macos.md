在Mac上，缺省bison/flex版本太低，需要重新安装。

```sh
brew install bison
echo 'export PATH="/usr/local/opt/bison/bin:$PATH"' >> ~/.zshrc

brew install flex
echo 'export PATH="/usr/local/opt/flex/bin:$PATH"' >> ~/.zshrc
```

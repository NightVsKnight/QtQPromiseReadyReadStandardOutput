# QtQPromiseReadyReadStandardOutput

**Shameless plug:** One way you can seriously help out me out is to subscribe to NightVsKnight's
[YouTube](https://www.youtube.com/channel/UCn8Ds6jeUzjxCPkMApg_koA) and/or
[Twitch](https://www.twitch.tv/nightvsknight) channels. I will be showing off my projects there from
time to time, and getting new subscribers gives me a little morale boost to help keep me going.

This project shows how `QProcess` strangely needs `QProcess::waitForFinished(...)` in order for
`QProcess::readyReadStandardError()` or `QProcess::readyReadStandardOutput()` to be called.

The main code is in `mainwindow.h`.

Yes, it isn't the best formatted code, but I was trying to keep all of the `class Pinger` code consolidated.

In `Pinger::startSync()`, if the only change I make is to comment out the call to
`QProcess::waitForFinished(...)` then `QProcess::readyReadStandardError()` or
`QProcess::readyReadStandardOutput()` never get called.

If the only change I make is to uncomment out that call to `QProcess::waitForFinished(...)` (so that
it is called) then `QProcess::readyReadStandardError()` or `QProcess::readyReadStandardOutput()` get
called just fine.

Why is that?

Also, why doesn't `QProcess::close()` work nicely in `Pinger::stopSync()`?

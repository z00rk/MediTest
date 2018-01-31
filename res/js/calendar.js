var CALENDAR = function () {
  var wrap, label, months = ["Janvier", "Fevrier", "Mars", "Avril", "May", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"];
  var year, month, today;

  function init (newWrap) {
    wrap = $(newWrap || "#main-layout-calendar");
    label = wrap.find("#label");

    today = new Date ();
    month = today.getMonth ();
    year  = today.getFullYear ();
    wrap.find ("#label").html (months[month]+" "+year);

    wrap.find ("#prev").bind ("click", function () {
                                         switchMonth (false, month, year);
                                       });
    wrap.find ("#next").bind ("click", function () {
                                         switchMonth (true, month, year);
                                       });

    createCalendar (year, month);
    label.click ();
  }

  function switchMonth (next, month, year) {
    var current = label.text ().trim ().split (" ");
    var tempYear = parseInt (current[1], 10);
    var tempMonth = months.indexOf (current[0]);

    if (next) {
      if (tempMonth == 11) {
        month = 0;
        year = tempYear + 1;
      } else {
        month = tempMonth + 1;
        year = tempYear;
      }
    } else {
      if (tempMonth == 0) {
        month = 11;
        year = tempYear -1;
      } else {
        month = tempMonth - 1;
        year = tempYear;
      }
    }

    label.html(months[month]+" "+year);

    calendar =  createCalendar(year, month);
  }

  function createCalendar (year, month) {
    var day = 1, i, j;
    var haveDay = true;

    var startDay = new Date (year, month, day).getDay ();
    var daysInMonth = [31, (((year % 4 == 0) && (year % 100) != 0)) || (year % 400) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    var calendar = [];

    console.log ("month "+month+" starts with "+startDay);
    i = 0;
    while (haveDay) {
      calendar[i] = [];
      for (j = 0; j < 7; j++) {
        if (i === 0) {
          if (j === startDay) {
            calendar[i][j] = day++;
            startDay++;
          }
        } else if (day <= daysInMonth[month]) {
          calendar[i][j] = day++;
        } else {
          calendar[i][j] = "";
          haveDay = false;
        }
      }
      i++;
    }

    if (calendar[5]) {
      for (i = 0; i < calendar[5].length; i++) {
        if (calendar[5][i] !== "") {
          calendar[4][i] = "<span>"+calendar[4][i]+"</span><span>"+calendar[5][i]+"</span>";
        }
      }
    }

    for (i = 0; i < calendar.length; i++) {
      calendar[i] = "<tr><td>"+calendar[i].join ("</td><td>")+"</td></tr>";
    }

    calendar = $("<table>"+calendar.join ("")+"</table>").addClass ("curr");
    $("td:empty", calendar).addClass ("nil");

    if (month === new Date ().getMonth ()) {
      $('td', calendar).filter (function () {
                                  return $(this).text () == new Date ().getDate ().toString ();
                                }).addClass ("today");
    }

    $("#main-layout-calendar-frame") 
      .find(".curr") 
      .removeClass("curr") 
      .addClass("temp") 
      .end() 
      .prepend(calendar) 
      .find(".temp") 
      .fadeOut("slow", function () {
                         $(this).remove(); }); 
    $('#label').text(calendar.label);
    return;
  }

  createCalendar.cache = {};
  return {
    init : init,
    switchMonth : switchMonth,
    createCalendar : createCalendar
  };
};
